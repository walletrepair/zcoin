#include <gtest/gtest.h>
#include <libzerocoin/sigma/R1Proof.h>
#include <libzerocoin/sigma/R1ProofGenerator.h>
#include <libzerocoin/sigma/R1ProofVerifier.h>
#include <stdlib.h>

bool test(secp_primitives::GroupElement& g, std::vector<secp_primitives::GroupElement>& h_, std::vector<secp_primitives::Scalar>& b, int n, int m){

    secp_primitives::Scalar r;
    r.randomize();
    zcoin_common::GeneratorVector<secp_primitives::Scalar, secp_primitives::GroupElement> h_gens(h_);
    sigma::R1ProofGenerator<secp_primitives::Scalar, secp_primitives::GroupElement> prover(g, h_gens, b, r, n, m);
    sigma::R1Proof<secp_primitives::Scalar, secp_primitives::GroupElement> proof;
    prover.proof(proof);

    sigma::R1ProofVerifier<secp_primitives::Scalar, secp_primitives::GroupElement> verifier(g,h_gens,prover.get_B(), n, m);
    return verifier.verify(proof);
}


TEST(sigma_R1_test, fixed_size_test)
{
    int n = 4;
    int m = 16;
    secp_primitives::GroupElement g;
    g.randomize();
    std::vector<secp_primitives::GroupElement> h_;
    std::vector<secp_primitives::Scalar> b;
    secp_primitives::GroupElement h;
    for(int i = 0; i < m; ++i) {
        h.randomize();
        h_.push_back(h);
        b.push_back(secp_primitives::Scalar(uint64_t(1)));
        for(int j = 1; j < n; ++j){
            h.randomize();
            h_.push_back(h);
            b.push_back(secp_primitives::Scalar(uint64_t(0)));

        }
    }

    EXPECT_TRUE(test(g, h_, b, n, m));
}

TEST(sigma_R1_test, random_size_test)
{
    int n = rand() % 64 + 16;
    int m = rand() % 32 + 16;
    secp_primitives::GroupElement g;
    g.randomize();
    std::vector<secp_primitives::GroupElement> h_;
    std::vector<secp_primitives::Scalar> b;
    for(int i = 0; i < m; ++i) {
        secp_primitives::GroupElement h;
        b.push_back(secp_primitives::Scalar(uint64_t(1)));
        for(int j = 1; j < n; ++j){
            h.randomize();
            h_.push_back(h);
            b.push_back(secp_primitives::Scalar(uint64_t(0)));

        }
    }

    EXPECT_TRUE(test(g, h_, b, n, m));
}


TEST(sigma_R1_test, all_positions)
{
    int n = 32;
    int m = 16;
    secp_primitives::GroupElement g;
    std::vector <secp_primitives::GroupElement> h_;
    std::vector <secp_primitives::Scalar> b;
    secp_primitives::GroupElement h;
    for(int k = 0; k < n; ++k){
        g.randomize();
        for(int i = 0;i< m ;++i) {
            for(int j = 0;j < n;++j){
                h.randomize();
                h_.push_back(h);
                if(j == k)
                    b.push_back(secp_primitives::Scalar(uint64_t(1)));
                else
                    b.push_back(secp_primitives::Scalar(uint64_t(0)));
            }
        }
        EXPECT_TRUE(test(g, h_, b, n, m));
        h_.clear();
        b.clear();
    }
}

TEST(sigma_R1_test, one_in_random_position)
{
    int n = 32;
    int m = 16;
    int k = rand() % n;
    secp_primitives::GroupElement g;
    std::vector <secp_primitives::GroupElement> h_;
    std::vector <secp_primitives::Scalar> b;
    secp_primitives::GroupElement h;
    g.randomize();
    for(int i = 0;i < m;++i) {
        k = rand() % n;
        for(int j = 0;j < n;++j){
            h.randomize();
            h_.push_back(h);
            if(j == k)
                b.push_back(secp_primitives::Scalar(uint64_t(1)));
            else
                b.push_back(secp_primitives::Scalar(uint64_t(0)));
        }
    }
    EXPECT_TRUE(test(g, h_, b, n, m));
}


TEST(sigma_R1_test, all_0s_in_random_row)
{
    int n = 32;
    int m = 16;
    int k = rand() % m;
    secp_primitives::GroupElement g;
    std::vector <secp_primitives::GroupElement> h_;
    std::vector <secp_primitives::Scalar> b;
    secp_primitives::GroupElement h;
    g.randomize();
    for(int i = 0;i < m;++i) {
        for(int j = 0;j < n;++j){
            h.randomize();
            h_.push_back(h);
            if(i != k)
                b.push_back(secp_primitives::Scalar(uint64_t(1)));
            else
                b.push_back(secp_primitives::Scalar(uint64_t(0)));
        }
    }
    EXPECT_FALSE(test(g, h_, b, n, m));
}

TEST(sigma_R1_test, all_1s_in_random_row)
{
    int n = 32;
    int m = 16;
    int k = rand() % m;
    secp_primitives::GroupElement g;
    std::vector <secp_primitives::GroupElement> h_;
    std::vector <secp_primitives::Scalar> b;
    secp_primitives::GroupElement h;
    g.randomize();
    for(int i = 0;i < m;++i) {
        for(int j = 0;j < n;++j){
            h.randomize();
            h_.push_back(h);
            if(i == k)
                b.push_back(secp_primitives::Scalar(uint64_t(1)));
            else
                b.push_back(secp_primitives::Scalar(uint64_t(0)));
        }
    }
    EXPECT_FALSE(test(g, h_, b, n, m));
}


TEST(sigma_R1_test, two_1s_in_random_row)
{
    int n = 32;
    int m = 16;
    int k = rand() % n;
    secp_primitives::GroupElement g;
    std::vector <secp_primitives::GroupElement> h_;
    std::vector <secp_primitives::Scalar> b;
    secp_primitives::GroupElement h;
    g.randomize();
    for(int i = 0;i < m;++i) {
        for(int j = 0;j < n;++j){
            h.randomize();
            h_.push_back(h);
            if(j == k){
                b.push_back(secp_primitives::Scalar(uint64_t(1)));
                b.push_back(secp_primitives::Scalar(uint64_t(1)));
                ++j;
            }
            else
                b.push_back(secp_primitives::Scalar(uint64_t(0)));
        }
    }
    EXPECT_FALSE(test(g, h_, b, n, m));
}
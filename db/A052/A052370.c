/*
 * A052370  Number of 6 X 6 binary matrices with n=0...36 ones up to row and
 *          column permutations.
 * https://oeis.org/A052370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   gcd_ll, lcm_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052370 – Number of 6×6 binary matrices with n ones,
   up to permutation of rows and columns.
   Formula: a(n) = (1/720²) ·
                 Σ_{σ∈S₆, τ∈S₆} [xⁿ]
                 ∏_{i∈Cycles(σ), j∈Cycles(τ)}
                     (1 + x^{lcm(i,j)})^{gcd(i,j)}       */

#include <stdint.h>   /* for size_t */
#include <string.h>   /* for memcpy */
#include "../utils/gcd_ll.h"
#include "../utils/lcm_ll.h"

/* ------------------------------------------------------------------ */
/*  auxiliary combinatorics                                           */
/* ------------------------------------------------------------------ */
static const unsigned long long C_binom[7][7] = {   /* binomial coeffs 0..6 */
    {1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0},
    {1,2,1,0,0,0,0},
    {1,3,3,1,0,0,0},
    {1,4,6,4,1,0,0},
    {1,5,10,10,5,1,0},
    {1,6,15,20,15,6,1}
};

/* ------------------------------------------------------------------ */
/*  cycle‑type representation                                         */
/* ------------------------------------------------------------------ */
struct Part {
    int  len;           /* number of cycles */
    int  cycles[6];     /* cycle lengths (unused beyond len) */
    unsigned long long cnt;   /* # of permutations of this type */
};

/* 11 possible cycle types of S₆ (non‑decreasing order of lengths)   */
static struct Part PARTS[11] = {
    {1,{6},0},
    {2,{5,1},0},
    {2,{4,2},0},
    {3,{4,1,1},0},
    {2,{3,3},0},
    {3,{3,2,1},0},
    {4,{3,1,1,1},0},
    {3,{2,2,2},0},
    {4,{2,2,1,1},0},
    {5,{2,1,1,1,1},0},
    {6,{1,1,1,1,1,1},0}
};

/* ------------------------------------------------------------------ */
/*  compute the numbers of permutations for each cycle type           */
/* ------------------------------------------------------------------ */
static void compute_perm_counts(void)
{
    static const unsigned long long fact[7] = {1,1,2,6,24,120,720};
    for (int p = 0; p < 11; ++p) {
        unsigned long long mult[7] = {0};
        for (int i = 0; i < PARTS[p].len; ++i)
            ++mult[ PARTS[p].cycles[i] ];
        unsigned long long cnt = fact[6];
        for (int l = 1; l <= 6; ++l) {
            if (mult[l] == 0) continue;
            unsigned long long pow_l = 1;
            for (unsigned long long k = 0; k < mult[l]; ++k) pow_l *= l;
            cnt /= pow_l;
            cnt /= fact[ mult[l] ];
        }
        PARTS[p].cnt = cnt;
    }
}

/* ------------------------------------------------------------------ */
/*  gcd and lcm for small positive integers                           */

/* ------------------------------------------------------------------ */
/*  main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* values exist only for 0 ≤ n ≤ 36 */
    if (n > 36U) return 0;

    /* static cache – computed once */
    static seq_t tbl[37] = {0};
    static int   ready  = 0;

    if (!ready) {
        /* pre‑compute permutation counts */
        compute_perm_counts();

        /* denominator |S₆ × S₆| = 720² = 518400 */
        const unsigned long long denom = 720ULL * 720ULL;

        unsigned long long sum[37] = {0};

        /* iterate over all pair of cycle types */
        for (int i = 0; i < 11; ++i) {           /* row type */
            for (int j = 0; j < 11; ++j) {       /* column type */
                unsigned long long weight = PARTS[i].cnt * PARTS[j].cnt;

                unsigned long long poly[37]   = {0};
                unsigned long long newpoly[37] = {0};
                poly[0] = 1;                    /* start with 1 */

                /* apply each pair of cycles (a,b) */
                for (int c1 = 0; c1 < PARTS[i].len; ++c1) {
                    int a = PARTS[i].cycles[c1];
                    for (int c2 = 0; c2 < PARTS[j].len; ++c2) {
                        int b = PARTS[j].cycles[c2];
                        unsigned long long g = gcd_ll((unsigned long long)a, (unsigned long long)b);
                        unsigned long long L = lcm_ll((unsigned long long)a, (unsigned long long)b);

                        /* multiply poly by (1 + x^L)^g */
                        memset(newpoly, 0, sizeof(newpoly));
                        for (int e = 0; e <= 36; ++e) if (poly[e]) {
                            for (unsigned long long k1 = 0; k1 <= g; ++k1) {
                                int idx = e + (int)(k1 * L);
                                if (idx > 36) break;
                                newpoly[idx] += poly[e] * C_binom[(int)g][(int)k1];
                            }
                        }
                        memcpy(poly, newpoly, sizeof(poly));
                    }
                }

                /* accumulate weighted polynomial */
                for (int k = 0; k <= 36; ++k)
                    sum[k] += weight * poly[k];
            }
        }

        /* divide by denominator to get final counts */
        for (int k = 0; k <= 36; ++k)
            tbl[k] = (seq_t)(sum[k] / denom);

        ready = 1;
    }

    return tbl[n];
}

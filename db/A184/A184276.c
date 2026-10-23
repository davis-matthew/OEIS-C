/*
 * A184276  Number of distinct n X 5 toroidal 0..3 arrays.
 * https://oeis.org/A184276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(5n)) * ( Σ_{g|n} φ(g) * 4^{5n/g} + 4 * Σ_{g|n} φ(g) *
 *            4^{n/g} if 5∤g, 4^{5n/g} if 5|g )
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements Burnside’s lemma for 4‑colourings of an n×5 torus.  */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, but safe */

    /* factorise n */
    idx_t primes[16];
    int   exps[16];
    size_t fcnt = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[fcnt] = p;
            exps[fcnt]   = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {
        primes[fcnt] = tmp;
        exps[fcnt]   = 1;
        ++fcnt;
    }

    /* generate all divisors g of n and φ(g) */
    idx_t divs[256];
    idx_t phis[256];
    size_t dcnt = 1;
    divs[0] = 1;
    phis[0] = 1;   /* φ(1) = 1 */

    for (size_t f = 0; f < fcnt; ++f) {
        idx_t p = primes[f];
        int   e = exps[f];
        idx_t p_pow[e + 1];
        p_pow[0] = 1;
        for (int i = 1; i <= e; ++i) p_pow[i] = p_pow[i - 1] * p;

        size_t old_cnt = dcnt;
        for (size_t i = 0; i < old_cnt; ++i) {
            for (int k = 1; k <= e; ++k) {
                idx_t new_div = divs[i] * p_pow[k];
                idx_t new_phi = phis[i];
                /* multiply φ by p^{k-1}*(p-1) */
                new_phi *= p_pow[k - 1] * (p - 1);
                divs[dcnt] = new_div;
                phis[dcnt] = new_phi;
                ++dcnt;
            }
        }
    }

    /* compute the two sums */
    seq_t sum1 = 0;
    seq_t sum2 = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t g   = divs[i];
        idx_t phi = phis[i];

        idx_t exp1 = 5 * n / g;
        idx_t exp2;
        if (g % 5 == 0)
            exp2 = 5 * n / g;
        else
            exp2 = n / g;

        seq_t pow1 = big_pow((seq_t)4, (seq_t)exp1);
        seq_t pow2 = big_pow((seq_t)4, (seq_t)exp2);

        sum1 += pow1 * (seq_t)phi;
        sum2 += pow2 * (seq_t)phi;
    }

    seq_t numerator = sum1 + (seq_t)4 * sum2;
    seq_t denom     = (seq_t)(5 * n);
    return numerator / denom;
}

/*
 * A192088  Number of permutations of [n] realized by the shift on 3 letters.
 * https://oeis.org/A192088
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-2) + Σ_{t=1}^{n-1} ψ₃(t)·3^(n-t-1) –
 *            (n-1)·Σ_{t=0}^{n-1} ψ₂(t)·2^(n-t-1) where ψ_N(t) = Σ_{d|t}
 *            μ(d)·N^(t/d) μ is the Möbius function
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/*  Memoised Möbius values up to the largest n requested.  */
static int  *mu  = NULL;
static idx_t mu_max = 0;

static void ensure_mu(idx_t n)
{
    if (n <= mu_max) return;
    int *nmu = (int*)malloc((n + 1) * sizeof(int));
    if (!nmu) exit(EXIT_FAILURE);
    nmu[0] = 0;
    nmu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t x = i;
        int  cnt = 0;
        int  p;
        int  square = 0;
        for (p = 2; (idx_t)p * p <= x; ++p) {
            if (x % p == 0) {
                int c = 0;
                while (x % p == 0) {
                    x /= p; ++c;
                    if (c > 1) { square = 1; break; }
                }
                if (square) break;
                ++cnt;
            }
        }
        if (x > 1) ++cnt;          /* remaining prime factor */
        nmu[i] = square ? 0 : ((cnt & 1) ? -1 : 1);
    }
    free(mu);
    mu     = nmu;
    mu_max = n;
}

/*  Primitive‑word count ψ_N(t).  */
static seq_t psiN(idx_t t, int N)
{
    if (t == 0) return (seq_t)0;
    seq_t res = (seq_t)0;
    for (idx_t d = 1; d * d <= t; ++d) {
        if (t % d == 0) {
            int mu_d = mu[d];
            if (mu_d != 0) {
                seq_t pow_val = big_pow((seq_t)N, t / d);
                res += pow_val * (seq_t)mu_d;
            }
            idx_t d2 = t / d;
            if (d2 != d) {
                int mu_d2 = mu[d2];
                if (mu_d2 != 0) {
                    seq_t pow_val = big_pow((seq_t)N, d);
                    res += pow_val * (seq_t)mu_d2;
                }
            }
        }
    }
    return res;
}

/*  Sequence term a(n) for n >= 2.  */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    ensure_mu(n);

    /* powers of 3 and 2 that will be reduced iteratively */
    seq_t pow3_cur = big_pow((seq_t)3, n - 2);   /* 3^(n-2) */
    seq_t pow2_cur = big_pow((seq_t)2, n - 2);   /* 2^(n-2) */

    seq_t sum3 = (seq_t)0;
    seq_t sum2 = (seq_t)0;

    for (idx_t t = 1; t < n; ++t) {
        /* Σ_{t=1}^{n-1} ψ₃(t)·3^(n-t-1) */
        sum3 += psiN(t, 3) * pow3_cur;
        pow3_cur = pow3_cur / (seq_t)3;          /* exponent decreases by 1 */

        /* Σ_{t=1}^{n-1} ψ₂(t)·2^(n-t-1)  (t=0 term is zero) */
        sum2 += psiN(t, 2) * pow2_cur;
        pow2_cur = pow2_cur / (seq_t)2;
    }

    seq_t pow3_term = big_pow((seq_t)3, n - 2);   /* 3^(n-2) part of formula */
    seq_t result = pow3_term + sum3;
    result -= ((seq_t)(n - 1)) * sum2;
    return result;
}

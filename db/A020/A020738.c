/*
 * A020738  Consider number of divisors of binomial(n, k), k=0..n; a(n) =
 *          multiplicity of the maximum value.
 * https://oeis.org/A020738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, mulmod61
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS formula (implicit) is
   a(n) = multiplicity of the largest value among
   d( binom(n,k) ) for k=0..n, where d(m) = number of divisors of m. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "../utils/build_spf.h"
#include "../utils/mulmod61.h"

/* Count the multiplicity of the maximum divisor count of the binomial
   coefficients in the n‑th row of Pascal's triangle. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* guard for offset */

    /* Smallest prime factor sieve up to n.  spf[i] is the smallest prime
       factor of i (0 <= i <= n).  build_spf allocates the array. */
    idx_t *spf = NULL;
    build_spf(n, &spf);

    /* Count primes up to n and build a quick index from prime to position
       in the exponent array. */
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (spf[i] == i) ++primes_cnt;

    int *pindex = (int *)malloc((n + 1) * sizeof(int));
    for (idx_t i = 0; i <= n; ++i) pindex[i] = -1;
    for (idx_t i = 2, idx = 0; i <= n; ++i)
        if (spf[i] == i) pindex[i] = (int)idx++;

    /* Exponent array for each prime (initially zero). */
    uint32_t *exp = (uint32_t *)calloc(primes_cnt, sizeof(uint32_t));

    /* Tracking structures for the current maximum divisor count. */
    long double max_log = -1.0L;
    const long double eps = 1e-18L;   /* tolerance for floating‑point comparison */
    idx_t maxCnt = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute log(divisor count) for the current coefficient.
           sum_log = Σ log(e_i + 1).  Also accumulate product modulo
           two large primes if more exact comparison were needed.  Here
           the logarithm suffices to order the values. */
        long double sum_log = 0.0L;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t e = exp[i];
            if (e) sum_log += logl((long double)(e + 1));
        }

        /* Update maximum statistics. */
        if (sum_log > max_log + eps) {
            max_log = sum_log;
            maxCnt = 1;
        } else if (fabsl(sum_log - max_log) <= eps) {
            /* The divisor counts are equal (up to rounding); 
               increment multiplicity. */
            ++maxCnt;
        }

        if (k == n) break;   /* last coefficient processed */

        /* Prepare for next k by multiplying by (n-k)/(k+1). */
        idx_t num = n - k;    /* numerator */
        idx_t den = k + 1;    /* denominator */

        /* Factor numerator: add exponents. */
        idx_t val = num;
        while (val > 1) {
            idx_t p = spf[val];
            int cnt = 0;
            while (val % p == 0) {
                val /= p;
                ++cnt;
            }
            exp[pindex[p]] += cnt;
        }

        /* Factor denominator: subtract exponents. */
        val = den;
        while (val > 1) {
            idx_t p = spf[val];
            int cnt = 0;
            while (val % p == 0) {
                val /= p;
                ++cnt;
            }
            exp[pindex[p]] -= cnt;
        }
    }

    /* Clean up. */
    free(exp);
    free(pindex);
    free(spf);

    return (seq_t)maxCnt;
}

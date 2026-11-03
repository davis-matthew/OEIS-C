/*
 * A124226  Number of partitions of n with even crank minus number of
 *          partitions of n with odd crank.
 * https://oeis.org/A124226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = coefficient of 2*x + ∏_{i≥1} (1-x^i)/(1+x^i)^2
 * Range:     i8 n <= 32, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline seq_t coeff_gi(idx_t m)
{
    /* Return coefficient of x^{i*m} in the factor
       g_i(x) = (1 - x^i)/(1+x^i)^2 = 1 + Σ_{m≥1} (-1)^m (2m+1) x^{i*m}   */
    seq_t w = (seq_t)(2*(seq_t)m + 1);
    if ((m & 1u) != 0u)                /* m is odd   →  (-1) factor  */
        w = -w;
    return w;
}

seq_t a(idx_t n)
{
    /* Allocate two arrays of length n+1 */
    seq_t *dp   = mem_new(seq_t, (n + 1));
    seq_t *next = mem_new(seq_t, (n + 1));
    if (!dp || !next) {                /* graceful failure if OOM */
        if (dp) mem_free(dp);
        if (next) mem_free(next);
        return (seq_t)0;
    }

    /* Initialise dp: product of zero factors = 1 */
    dp[0] = (seq_t)1;
    for (idx_t t = 1; t <= n; ++t)
        dp[t] = (seq_t)0;

    /* Multiply successively by each factor g_i(x) */
    for (idx_t i = 1; i <= n; ++i) {
        /* start with a copy of the old dp */
        for (idx_t t = 0; t <= n; ++t)
            next[t] = dp[t];

        /* add contributions from powers of x^i with coefficients (-1)^m(2m+1) */
        for (idx_t t = 1; t <= n; ++t) {
            idx_t maxm = t / i;
            for (idx_t m = 1; m <= maxm; ++m) {
                seq_t w = coeff_gi(m);
                next[t] += w * dp[t - m * i];
            }
        }

        /* copy new coefficients back into dp */
        for (idx_t t = 0; t <= n; ++t)
            dp[t] = next[t];
    }

    /* Final result: coefficient of x^n in the product, plus   */
    /*   the extra term 2*x (only contributes for n==1)       */
    seq_t result = dp[n];
    if (n == 1)
        result += (seq_t)2;

    mem_free(dp);
    mem_free(next);
    return result;
}

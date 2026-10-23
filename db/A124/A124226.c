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
 * Helpers:   coeff_gi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/coeff_gi.h"

seq_t a(idx_t n)
{
    /* Allocate two arrays of length n+1 */
    seq_t *dp   = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp || !next) {                /* graceful failure if OOM */
        if (dp) free(dp);
        if (next) free(next);
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

    free(dp);
    free(next);
    return result;
}

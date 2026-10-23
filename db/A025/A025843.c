/*
 * A025843  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A025843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 6, and 10
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming – for each coin add contributions to higher sums. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a dynamic array dp[0..n] and initialise to 0. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) abort();               /* allocation failure */

    dp[0] = (seq_t)1;                /* base case: one way to partition 0 */

    const idx_t parts[3] = {3, 6, 10};
    for (int p = 0; p < 3; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

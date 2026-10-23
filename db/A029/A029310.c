/*
 * A029310  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 7, 9 and 10
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via standard unbounded knapsack DP (coin change counting) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The DP array has length n+1.  Use calloc to zero-initialize it. */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1U, sizeof(seq_t));
    if (dp == NULL) {
        /* In case of allocation failure, return 0 (undefined behavior is rare). */
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;                   /* base case: one way to sum to 0 */

    const idx_t parts[4] = {3, 7, 9, 10};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t k = p; k <= n; ++k) {
            dp[k] += dp[k - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

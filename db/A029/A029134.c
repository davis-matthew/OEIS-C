/*
 * A029134  Expansion of 1/((1-x)(1-x^9)(1-x^11)(1-x^12)).
 * https://oeis.org/A029134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of partitions of n into parts 1, 9, 11, and
 *            12
 * Range:     u8 n <= 105, u16 n <= 759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    static const idx_t parts[4] = {1ULL, 9ULL, 11ULL, 12ULL};

    /* Allocate DP array of length n+1. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;                 /* allocation failure */

    /* Initialise: dp[0] = 1 (empty partition), others 0. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Dynamic programming: for each part add contributions to larger indices. */
    for (int k = 0; k < 4; ++k) {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i) dp[i] += dp[i - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

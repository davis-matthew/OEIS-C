/*
 * A029329  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A029329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 5, 6 and 10
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Exact sequence type definitions are supplied by the build. */
static const idx_t parts[4] = {4, 5, 6, 10};

seq_t a(idx_t n)
{
    /* Allocate an array for DP values [0..n]. The first element dp[0] is 1. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (dp == NULL) abort();
    dp[0] = 1;

    /* Classic coin‑change DP: outer loop over parts, inner loop over sums. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

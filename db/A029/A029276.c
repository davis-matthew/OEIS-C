/*
 * A029276  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 6 and 11
 * Range:     u8 n <= 103, u16 n <= 717, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* The part sizes. */
    const idx_t parts[4] = { 3, 5, 6, 11 };

    /* Allocate a DP array of size n+1.  The cast to size_t is safe for
       all reasonable n (the sequence is not defined for astronomically
       large indices). */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)
        return (seq_t)0; /* memory allocation failed */

    /* Initialize dp[0] and all other entries to 0. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Standard unbounded knapsack counting: for each part, add its
       contribution to all larger sums. */
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

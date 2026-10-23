/*
 * A029210  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2,5,9,12.
 *            Computation using O(n) DP with fixed part sizes
 * Range:     u8 n <= 104, u16 n <= 737, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const idx_t denoms[4] = {2, 5, 9, 12};

    /* Allocate a zero‑initialised array of length n+1 */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp)
        exit(1);   /* allocation failed */

    dp[0] = 1;
    for (idx_t coin_index = 0; coin_index < 4; ++coin_index) {
        idx_t coin = denoms[coin_index];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

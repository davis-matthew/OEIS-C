/*
 * A029146  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A029146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 464, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029146: number of partitions of n into parts 2, 3, 5, and 9 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate an array of size n+1 to hold the DP values */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) abort();

    dp[0] = (seq_t)1;
    const idx_t coins[4] = {2, 3, 5, 9};

    /* Coin‑change DP: number of unordered partitions */
    for (size_t ci = 0; ci < 4; ++ci)
    {
        idx_t coin = coins[ci];
        for (idx_t i = coin; i <= n; ++i)
        {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

/*
 * A029384  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts 5, 6, 8 and 10
 * Range:     u8 n <= 137, u16 n <= 963, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a DP array of size n+1.
       The array is small (≤ 10001 for the intended tests). */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0; /* unlikely, but prevents crashes */

    /* Base case: one way to make sum 0 (use no parts). */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* The coin values available. */
    const idx_t coins[4] = {5, 6, 8, 10};

    /* Standard unbounded knapsack DP for counting combinations. */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

/*
 * A029173  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^5)*(1-x^8)).
 * https://oeis.org/A029173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 4, 5, and 8
 *            (coin‑change DP)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Allocate an array of n+1 seq_t elements.  */
    seq_t *dp;
#if defined(__cplusplus)
    dp = new seq_t[n + 1];
#else
    dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;           /* allocation failure – caller must handle */
#endif

    /* Initialise dp[0] = 1, the rest = 0.  */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Parts allowed: 2, 4, 5, 8.  */
    const idx_t coins[4] = {2, 4, 5, 8};

    /* Count partitions: for each coin, add the ways to reach higher amounts. */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}

/*
 * A029207  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 707, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029207
   Number of partitions of n into parts 2, 5, 8, and 12.
   Computed by a classic coin‑change DP in O(n) time. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* allocate a temporary array of size n+1 */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1];
#else
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) abort();
#endif

    /* initialise all entries to 0 and set dp[0] = 1 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* coin values */
    const idx_t coins[4] = {2, 5, 8, 12};

    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t ans = dp[n];

    /* free the temporary array */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return ans;
}

/*
 * A029379  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029379: Partition number of n into parts 5, 6, 7, 9
   Computed by a classic coin‑change DP (order does not matter). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return 0;          /* Allocation failed */

    /* Initialization: only zero is representable by no parts */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;

    /* Coin set {5,6,7,9} – order of coins is irrelevant for combinations */
    static const idx_t coins[4] = {5, 6, 7, 9};
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}

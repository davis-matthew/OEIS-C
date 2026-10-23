/*
 * A029357  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 6, 10, and 11. We
 *            compute it by the standard coin‑change dynamic programming
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate a temporary array for the DP table. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return 0;  /* memory failure – unlikely for n ≤ 10000 */

    /* Initialise: only 0 can be made in one way. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Coin denominations.  Order does not matter here because each coin
       is processed once and the inner loop runs i from coin to n increasing
       which guarantees unordered partitions. */
    const idx_t coins[4] = {4, 6, 10, 11};

    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

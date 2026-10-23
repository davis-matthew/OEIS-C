/*
 * A029228  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 7, 8, and 11
 * Range:     u8 n <= 109, u16 n <= 771, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Parts allowed */
    static const idx_t COINS[4] = {2, 7, 8, 11};

    /* Allocate DP table, length n+1 */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0; /* memory failure – return 0 as a safe default */

    /* dp[0] = 1 (empty sum). Initialize others to 0. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Standard unlimited‑coins DP: for each coin, add contributions. */
    for (idx_t c = 0; c < 4; ++c) {
        idx_t coin = COINS[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    /* Result for the requested n */
    seq_t result = dp[n];
    free(dp);
    return result;
}

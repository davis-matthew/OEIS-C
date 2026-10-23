/*
 * A029393  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into parts 5, 7, 8,
 *   and 9.  The values are computed by the standard coin‑change
 *   DP:  dp[0] = 1;  for each coin c, for i = c .. n: dp[i] += dp[i‑c].
 *   This is a direct O(n) algorithm and works for any order of calls.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary array for the DP table. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (dp == NULL) {
        return 0;          /* allocation failed – return 0 as a safe value */
    }

    /* Initialise the DP table. */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;

    /* Coin denominations. */
    static const idx_t coins[4] = {5, 7, 8, 9};

    /* Compute partitions: for each coin, update all reachable sums. */
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

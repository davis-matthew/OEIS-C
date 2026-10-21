/*
 * A067996  Number of ways of making change for n cents using coins of 1, 2,
 *          3, 5, 10, 20, 25, 50, 100 cents (all historical U.S.A. coinage
 *          from 1 to 100 cents).
 * https://oeis.org/A067996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of ways to make change for n cents using coins 1,
 *            2, 3, 5, 10, 20, 25, 50, 100 (order of coins does not matter)
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* one way to make 0 cents: no coins */

    /* Coin denominations available. */
    const idx_t coins[] = {1, 2, 3, 5, 10, 20, 25, 50, 100};
    const size_t coin_cnt = sizeof coins / sizeof *coins;

    /* DP array: dp[i] = number of ways to make i cents. */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failure; unlikely for n<=10000 */

    dp[0] = (seq_t)1;  /* base case */

    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t amt = coin; amt <= n; ++amt) {
            dp[amt] += dp[amt - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

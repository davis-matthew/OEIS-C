/*
 * A057537  Number of ways of making change for n Euro-cents using the Euro
 *          currency.
 * https://oeis.org/A057537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of ways to make change for n Euro‑cents using the Euro
 *            currency (denominations 1, 2, 5, 10, 20, 50, 100, 200, 500,
 *            1000, 2000, 5000, 10000, 20000, 50000)
 * Range:     u8 n <= 41, u16 n <= 194, u32 n <= 1506, u64 n <= 9468,
 *            u128 n <= 9468, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Euro coin denominations (cents). */
    static const idx_t COINS[] = {1, 2, 5, 10, 20, 50, 100, 200, 500,
                                 1000, 2000, 5000, 10000, 20000, 50000};
    const size_t NUM_COINS = sizeof(COINS) / sizeof(COINS[0]);

    /* Allocate DP table. */
    seq_t *dp = alloc_seq_array((size_t)n + 1);
    if (!dp)
        return (seq_t)0;    /* Allocation failure – unlikely for n ≤ 10 000. */

    /* Initialise DP array to zero, then set base case. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Classic coin‑change dynamic programming. */
    for (size_t c = 0; c < NUM_COINS; ++c) {
        idx_t coin = COINS[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}

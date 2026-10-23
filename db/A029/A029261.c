/*
 * A029261  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A029261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3,4,7,12
 * Range:     u8 n <= 102, u16 n <= 721, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached DP table */
static seq_t *dp = NULL;
static idx_t dp_size = 0;

/* External function required by the framework */
seq_t a(idx_t n)
{
    /* Ensure the DP table is large enough for index n */
    if (n + (idx_t)1 > dp_size) {
        /* Resize and recompute */
        if (dp != NULL) {
            free(dp);
        }
        dp_size = n + (idx_t)1;
        dp = (seq_t*)malloc((size_t)dp_size * sizeof(seq_t));
        if (dp == NULL) {              /* allocation failure: return 0 */
            return 0;
        }

        /* Initialize */
        for (idx_t i = 0; i < dp_size; ++i) {
            dp[i] = 0;
        }
        dp[0] = 1;                      /* one way to partition 0 */

        /* Coin denominations */
        const idx_t coins[4] = {3, 4, 7, 12};

        /* Classic unbounded knapsack DP for partitions */
        for (int ci = 0; ci < 4; ++ci) {
            idx_t coin = coins[ci];
            for (idx_t i = coin; i < dp_size; ++i) {
                dp[i] += dp[i - coin];
            }
        }
    }

    return dp[n];
}

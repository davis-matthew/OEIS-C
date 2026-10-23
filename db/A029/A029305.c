/*
 * A029305  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 6, 11, 12
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by the standard coin‑change DP: dp[0] = 1; for each coin c,
   for i = c … n: dp[i] += dp[i‑c]. */

#include <stdlib.h>

static seq_t *dp          = NULL;   /* cache: dp[i] = a(i) for i < dp_capacity */
static idx_t  dp_capacity = 0;      /* size of allocated cache */

/* Return a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Rebuild cache if the requested n is larger than current capacity. */
    if (n + 1 > dp_capacity) {
        /* Allocate new array, zeroed. */
        seq_t *new_dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!new_dp) return 0;      /* allocation failure: return 0   */

        /* Compute the DP values up to n. */
        new_dp[0] = 1;              /* a(0) = 1 */

        /* Coin denominations. */
        static const idx_t coins[4] = {3, 6, 11, 12};

        for (int ci = 0; ci < 4; ++ci) {
            idx_t c = coins[ci];
            for (idx_t i = c; i <= n; ++i) {
                new_dp[i] += new_dp[i - c];
            }
        }

        /* Replace old cache. */
        free(dp);
        dp          = new_dp;
        dp_capacity = n + 1;
    }

    /* dp[n] is now guaranteed to have been computed. */
    return dp[n];
}

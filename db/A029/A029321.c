/*
 * A029321  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the number of partitions of n into the parts
   3, 8, 11, and 12.  It is computed by the classic
   unlimited‑coin–change dynamic programming algorithm:
   dp[0] = 1; for each coin c, for all j >= c, dp[j] += dp[j - c].
   This counts each unordered multiset of parts exactly once. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *dp = NULL;
static idx_t dp_len = 0;

/* Rebuilds the DP table up to and including max_n. */
static void rebuild_dp(idx_t max_n)
{
    /* Allocate new table. */
    idx_t new_len = max_n + 1;
    seq_t *new_dp  = alloc_seq_array(new_len);

    /* Initialise all entries to zero and set the base case. */
    for (idx_t i = 0; i < new_len; ++i)
        new_dp[i] = 0;
    new_dp[0] = 1;

    /* The four allowable parts. */
    const idx_t coins[4] = {3, 8, 11, 12};

    /* Standard coin‑change DP: iterate over coins first. */
    for (size_t k = 0; k < 4; ++k) {
        idx_t coin = coins[k];
        for (idx_t j = coin; j < new_len; ++j) {
            new_dp[j] += new_dp[j - coin];
        }
    }

    /* Replace the old table. */
    if (dp != NULL)
        free_seq_array(dp);
    dp = new_dp;
    dp_len = new_len;
}

/* Public interface. */
seq_t a(idx_t n)
{
    if (dp == NULL || n + 1 > dp_len)
        rebuild_dp(n);
    return dp[n];
}

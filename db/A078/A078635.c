/*
 * A078635  Number of partitions of n into perfect powers.
 * https://oeis.org/A078635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into perfect powers. (Using
 *            dynamic programming with coin change over all distinct perfect
 *            powers.)
 * Range:     u8 n <= 48, u16 n <= 155, u32 n <= 616, u64 n <= 2976,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_powers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/generate_powers.h"

static seq_t *dp = NULL;          /* DP array */
static idx_t  max_computed = 0;   /* maximum n for which dp was computed */

/* Recompute dp array up to new_limit using all perfect powers <= new_limit. */
static void recompute_dp(idx_t new_limit)
{
    /* Allocate new dp array */
    dp = (seq_t*)calloc((size_t)(new_limit + 1), sizeof(seq_t));
    if (!dp) return;  /* allocation failed – unlikely */

    dp[0] = (seq_t)1;   /* base case: one way to partition 0 */

    /* Generate perfect powers up to new_limit */
    idx_t *powers = NULL;
    size_t p_len = 0, p_cap = 0;
    generate_powers(new_limit, &powers, &p_len, &p_cap);

    /* Simple coin–change DP */
    for (size_t p_idx = 0; p_idx < p_len; ++p_idx) {
        idx_t coin = powers[p_idx];
        for (idx_t s = coin; s <= new_limit; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    free(powers);
    max_computed = new_limit;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n > max_computed || dp == NULL) {
        recompute_dp(n);
    }
    return dp[(size_t)n];
}

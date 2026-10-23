/*
 * A029286  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 9, and 10
 * Range:     u8 n <= 113, u16 n <= 796, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Protect against overflow when converting to size_t for allocation. */
    if (n > SIZE_MAX - 1)
        return (seq_t)0;

    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    if (!dp)
        return (seq_t)0;

    /* Initialise DP array. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* one way to partition 0 */

    /* Part sizes allowed in the partition. */
    const idx_t coins[4] = {3, 5, 9, 10};

    /* Dynamic‑programming: for each coin, update all reachable sums. */
    for (int k = 0; k < 4; ++k) {
        idx_t c = coins[k];
        for (idx_t j = c; j <= n; ++j) {
            dp[j] += dp[j - c];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}

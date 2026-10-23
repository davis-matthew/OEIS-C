/*
 * A029214  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 627, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into parts 2, 6, 7, and 8. */
/* Computed with a simple 1‑dimensional DP for each call. */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of length n+1. */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));

    /* Initialise the DP array. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Coin values. */
    const idx_t coins[4] = { 2, 6, 7, 8 };

    /* Standard one‑pass DP. */
    for (int ci = 0; ci < 4; ++ci) {
        idx_t c = coins[ci];
        for (idx_t i = c; i <= n; ++i)
            dp[i] += dp[i - c];
    }

    seq_t result = dp[n];

    free_seq_array(dp);
    return result;
}

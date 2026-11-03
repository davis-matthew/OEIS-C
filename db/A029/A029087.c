/*
 * A029087  Expansion of 1/((1-x)*(1-x^5)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 5, 6, and 9
 *            (computed by dynamic programming in O(n) time)
 * Range:     u8 n <= 64, u16 n <= 463, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 929 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case explicitly */
    if (n == 0) return 1;

    /* Allocate a zero‑initialised array of size n+1. */
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    if (!dp) return 0;          /* memory exhaustion – unlikely in tests */

    dp[0] = 1;                  /* one way to partition 0: use no parts */

    /* Coins available: 1, 5, 6, and 9. */
    const idx_t coins[4] = {1, 5, 6, 9};

    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        if (coin > n) continue; /* skip if coin value exceeds current n */

        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}

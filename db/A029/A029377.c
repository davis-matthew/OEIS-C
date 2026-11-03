/*
 * A029377  Expansion of 1/((1-x^4)*(1-x^10)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4,10,11,12
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static const idx_t coins[4] = {4, 10, 11, 12};
    /* Allocate DP array */
    seq_t *dp = mem_new(seq_t, (n + 1));
    if (!dp) return 0;           /* unlikely, but safe */

    /* Initialize */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* DP computation */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] = dp[i] + dp[i - coin];
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}

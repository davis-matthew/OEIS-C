/*
 * A001156  Number of partitions of n into squares.
 * https://oeis.org/A001156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: Product_{m>=1} 1/(1-x^(m^2)); a(n) computed by the
 *            coin-change DP over squares, a(0) = 1
 * Range:     u8 n <= 66, u16 n <= 234, u32 n <= 1007, u64 n <= 5179,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

seq_t a(idx_t n) {
    seq_t *dp = alloc_zero(n + 1);
    dp[0] = 1;

    for (idx_t m = 1; m * m <= n; m++) {
        idx_t sq = m * m;
        for (idx_t i = sq; i <= n; i++) {
            dp[i] += dp[i - sq];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

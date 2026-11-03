/*
 * A029303  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 6, 10, and 11
 *            Computed by the classic coin-change (unbounded knapsack) DP:
 *            dp[i] = sum_{a in {3,6,10,11}, a <= i} dp[i-a], with dp[0]=1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    seq_t *dp = mem_new(seq_t, (n + 1));
    if (!dp) return 0;          /* allocation failure, return 0 as safe fallback */

    /* Initialize dp[0] = 1 and others to 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Allowed part sizes */
    static const idx_t parts[4] = {3, 6, 10, 11};

    /* Unbounded coin-change DP */
    for (int p = 0; p < 4; ++p) {
        idx_t a = parts[p];
        for (idx_t i = a; i <= n; ++i) {
            dp[i] += dp[i - a];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}

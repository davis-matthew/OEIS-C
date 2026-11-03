/*
 * A025799  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^10)).
 * https://oeis.org/A025799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts 2, 3 and 10. a(n) = #{
 *            (a,b,c) ∈ ℕ³ : 2a + 3b + 10c = n } Computed by the classic
 *            unbounded‑coin change DP (O(n) time)
 * Range:     u8 n <= 167, u16 n <= 2795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9735 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* allocate DP array of length n+1 and initialise to 0 */
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    if (!dp) return 0;             /* fallback in case of allocation failure */

    dp[0] = 1;                     /* one way to make 0 */

    /* unbounded coins: 2, 3, and 10 */
    const idx_t coins[3] = {2, 3, 10};
    for (int k = 0; k < 3; ++k) {
        idx_t coin = coins[k];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}

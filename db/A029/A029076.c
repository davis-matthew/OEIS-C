/*
 * A029076  Expansion of 1/((1-x)(1-x^4)(1-x^7)(1-x^11)).
 * https://oeis.org/A029076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 4, 7, and 11. The
 *            sequence is obtained by the standard coin‑change dynamic
 *            programming: dp[0] = 1, dp[i] = Σ dp[i – part] for all allowed
 *            parts ≤ i
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t _parts[4] = {1, 4, 7, 11};

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t allocated_n = 0;

    /* Allocate/resize the DP array for the requested n. */
    if (dp == NULL || allocated_n < n) {
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + (idx_t)1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        dp = tmp;
        allocated_n = n;
    }

    /* Clear the entries up to n and set dp[0] = 1. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Coin‑change DP: for each part, add its contribution for all sums. */
    for (int p_idx = 0; p_idx < 4; ++p_idx) {
        idx_t part = _parts[p_idx];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}

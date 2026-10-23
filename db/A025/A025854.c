/*
 * A025854  Expansion of 1/((1-x^3)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A025854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of partitions of n into parts 3, 9, and 10
   using the standard unbounded coin‑change DP. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate array for DP values */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;                 /* allocation failed – return 0 */

    /* Clear the table and set dp[0] = 1 (empty partition) */
    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Unbounded coin change for parts 3, 9, 10 */
    const idx_t parts[3] = {3, 9, 10};
    for (int p = 0; p < 3; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

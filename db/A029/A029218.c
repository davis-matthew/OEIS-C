/*
 * A029218  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A029218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into the parts 2, 6, 7 and 12 (OEIS
 *            A029218)
 * Range:     u8 n <= 99, u16 n <= 717, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Special case: n == 0 gives the empty partition */
    if (n == 0) return (seq_t)1;

    const idx_t parts[4] = {2, 6, 7, 12};
    /* Allocate DP array */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failed */

    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* one way to make sum 0 */

    /* Unbounded coin change DP */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

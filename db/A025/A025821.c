/*
 * A025821  Expansion of 1/((1-x^2)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A025821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 8, and 9
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming (coin change). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) return 1;

    /* allocate dp[0..n]; cast malloc result for C++ compatibility */
    seq_t *dp = (seq_t*)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* allocation failure */

    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    const idx_t parts[3] = {2, 8, 9};
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

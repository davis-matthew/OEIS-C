/*
 * A025889  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A025889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 5, 8, and 11
 * Range:     u8 n <= 462, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a DP array of size n+1 */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;           /* Allocation failure – unlikely for small n */

    /* Initialise DP array */
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Incrementally count partitions using the three part sizes */
    const idx_t parts[3] = {5, 8, 11};
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

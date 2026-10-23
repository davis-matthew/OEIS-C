/*
 * A025841  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A025841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 3, 6, and 7 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a temporary array for dynamic programming.  The array
       holds the number of partitions of each k ≤ n.  */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) exit(1);

    /* Initialise to zero; dp[0] will be set to 1. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* The coin‑change algorithm that counts unordered partitions. */
    const idx_t parts[3] = {3, 6, 7};
    for (int k = 0; k < 3; ++k) {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

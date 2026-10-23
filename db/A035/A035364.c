/*
 * A035364  Number of partitions of n into parts 4k or 4k+3.
 * https://oeis.org/A035364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 4k or 4k+3.  A standard
   coin‑change dynamic programming is used:  the parts are treated
   as coin types, and for each type we add its contribution to all
   larger sums.  The function recomputes the table for every call
   (O(n²/4) additions), which is fast enough for the required limits. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) is the first printed term. */
    if (n == 0)
        return 0;  /* undefined, return 0 for safety */

    /* Allocate DP table of length n+1. */
    size_t size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp)
        return 0;  /* allocation failed → return 0 as a safe fallback */

    /* Initialisation: only the empty partition contributes to sum 0. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Parts that are multiples of 4 (4k). */
    for (idx_t p = 4; p <= n; p += 4) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    /* Parts that are 4k+3. */
    for (idx_t p = 3; p <= n; p += 4) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

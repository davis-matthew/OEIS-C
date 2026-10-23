/*
 * A035460  Number of partitions of n into parts 8k+3 or 8k+5.
 * https://oeis.org/A035460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 177, u32 n <= 509, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A035460: partitions of n into parts 8k+3 or 8k+5.
   Computed by the standard unbounded coin‑change DP:
   dp[0] = 1; for each allowed part p (3,5,11,13,…) do
       for i = p .. n: dp[i] += dp[i - p]   */
/* This implementation uses a fresh array per call and therefore
   works for any build of seq_t without special allocation routines. */

#include <stdlib.h>
#include <string.h>   /* for memset */

seq_t a(idx_t n)
{
    /* allocate array for DP values */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* out of memory – return 0 */

    /* initialise to zero and set base case */
    memset(dp, 0, len * sizeof(seq_t));
    dp[0] = (seq_t)1;

    /* contributions from parts of the form 8k+3 */
    for (idx_t p = 3; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    /* contributions from parts of the form 8k+5 */
    for (idx_t p = 5; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

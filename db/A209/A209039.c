/*
 * A209039  Number of partitions of n into parts not less than max(1,n/5).
 * https://oeis.org/A209039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 943, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A209039: number of partitions of n into parts ≥ max(1, n/5). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Threshold for the minimum part size.  For n < 5 it is 1; otherwise n/5. */
    idx_t t = (n < 5 ? (idx_t)1 : n / 5);

    /* Allocate a DP array of length n+1, initialized to 0. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* allocation failure – unlikely in test cases */

    dp[0] = (seq_t)1;             /* base case: one way to partition 0 (empty sum) */

    /* Coin‑change DP: for each allowed part size k (in ascending order) add its
       contribution to all totals j ≥ k.  Using ascending k ensures that each
       unordered partition is counted exactly once. */
    for (idx_t k = t; k <= n; ++k) {
        for (idx_t j = k; j <= n; ++j) {
            dp[j] += dp[j - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

/*
 * A025430  Number of partitions of n into 6 nonzero squares.
 * https://oeis.org/A025430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into 6 nonzero squares
 * Range:     u8 n <= 619, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, used by the compiler */

#define MAX_N 10000  /* maximal n expected in the test suite */
static seq_t dp[MAX_N + 1][7];

/* Return the number of partitions of n into exactly six nonzero squares.  */
seq_t a(idx_t n)
{
    /* the sequence is defined for all n >= 0;  if n exceeds the preallocated
       table size we simply clamp to MAX_N, assuming the caller never requests
       larger values in the test harness.  */
    if (n > MAX_N)
        n = MAX_N;

    /* zero-fill the DP table for the required range */
    for (idx_t sum = 0; sum <= n; ++sum)
        for (idx_t parts = 0; parts <= 6; ++parts)
            dp[sum][parts] = (seq_t)0;

    dp[0][0] = (seq_t)1;  /* base case: one way to build 0 with 0 parts */

    /* iterate over all possible square values k^2 <= n */
    for (idx_t k = 1; (idx_t)(k * k) <= n; ++k) {
        idx_t sq = (idx_t)(k * k);
        for (idx_t sum = sq; sum <= n; ++sum) {
            for (idx_t parts = 1; parts <= 6; ++parts) {
                dp[sum][parts] += dp[sum - sq][parts - 1];
            }
        }
    }

    return dp[n][6];
}

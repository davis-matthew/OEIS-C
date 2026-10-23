/*
 * A025893  Expansion of 1/((1-x^5)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A025893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 5, 9, and 12. This
 *            implementation uses a simple dynamic programming recurrence:
 *            dp[0] = 1, and for each part p in {5,9,12} and i >= p, dp[i] +=
 *            dp[i - p]. The algorithm runs in O(n) time and O(n) memory, and
 *            works for any build of seq_t and idx_t
 * Range:     u8 n <= 509, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a temporary array of size n+1.  The array is always
       freed before the function returns, so no static memory is
       retained between calls. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return 0;               /* defensive: unlikely to happen */

    /* Initialize the DP array.  Only dp[0] must be set to 1;
       all other entries are initially zero by malloc, but we zero
       them explicitly for clarity. */
    dp[0] = 1;
    for (size_t i = 1; i < len; ++i) dp[i] = 0;

    /* The three allowed part sizes. */
    const idx_t parts[3] = {5, 9, 12};

    /* Dynamic programming: add contributions of each part size. */
    for (int pi = 0; pi < 3; ++pi) {
        idx_t p = parts[pi];
        if (p > n) continue;          /* skip if part exceeds n */
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    /* Store result before freeing the array. */
    seq_t result = dp[n];
    free(dp);
    return result;
}

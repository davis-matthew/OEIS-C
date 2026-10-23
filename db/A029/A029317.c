/*
 * A029317  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implements: a(n) = number of partitions of n into parts 3, 8, 9, and 11.
 * The function uses an unbounded knapsack (dynamic programming) with four part
 * sizes.  It runs in O(n) time and O(n) memory, which is well within the
 * required limits for n ≤ 10,000.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case first. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate a dynamic array of length n+1.  Use size_t for the allocation
       size to avoid warnings when n is large.  All arithmetic uses idx_t
       to match the type of the index. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) {
        /* Allocation failed – return 0 as a safe sentinel. */
        return (seq_t)0;
    }

    /* Initialise the DP array: dp[0] = 1 (empty partition), others 0. */
    for (idx_t i = (idx_t)0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    /* Allowed part sizes.  The array is small so we can use a compile‑time
       constant for its length. */
    const idx_t parts[] = {3, 8, 9, 11};
    const size_t part_cnt = sizeof(parts) / sizeof(parts[0]);

    /* Unbounded knapsack DP: for each part, add its contribution to all
       reachable sums.  Since the part sizes are fixed we can iterate
       with simple loops. */
    for (size_t pi = (size_t)0; pi < part_cnt; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    /* The answer is stored in dp[n].  Clean up and return. */
    seq_t result = dp[n];
    free(dp);
    return result;
}

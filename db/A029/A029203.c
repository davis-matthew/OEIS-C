/*
 * A029203  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A029203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 678, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029203: partitions of n into parts 2, 5, 7, and 12
   Uses a simple DP: dp[0] = 1; for each part p
   for i = p .. n: dp[i] += dp[i-p];
   This algorithm counts unordered partitions. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Parts allowed in the partition. */
    static const idx_t parts[4] = {2, 5, 7, 12};

    /* Memoisation array: dp[i] holds the number of partitions of i. */
    static seq_t *dp = NULL;      /* array of partition counts */
    static size_t dp_size = 0;    /* allocated size of dp */

    /* Allocate or grow the array to accommodate indices up to n. */
    if (dp == NULL || (size_t)(n + 1) > dp_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failed */
        dp = tmp;
        dp_size = new_size;
    }

    /* Initialize dp[0..n] to zero.  A loop is used so that the code works
       for arbitrary-precision seq_t types. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;  /* base case: one way to partition 0 */

    /* Dynamic programming: for each allowed part, update all positions. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        if (p > n) continue;                /* part larger than n is useless */
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}

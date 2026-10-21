/*
 * A078134  Number of ways to write n as sum of squares > 1.
 * https://oeis.org/A078134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into squares > 1. Computation
 *            by dynamic programming over the squares k² (k ≥ 2)
 * Range:     u8 n <= 156, u16 n <= 444, u32 n <= 1588, u64 n <= 7061,
 *            u128 n <= 10000, big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate table for DP values (0 … n).  The array holds seq_t values. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = seq_alloc_new_array(sz);

    /* Initialise with zeros and set dp[0] = 1 (empty sum). */
    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Iterate over all square numbers k² with k ≥ 2. */
    for (idx_t k = 2;; ++k)
    {
        idx_t sq = k * k;
        if (sq > n) break;
        for (idx_t i = sq; i <= n; ++i)
            dp[i] += dp[i - sq];
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}

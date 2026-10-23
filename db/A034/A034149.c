/*
 * A034149  Number of partitions of n into distinct parts from [1, 19].
 * https://oeis.org/A034149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts from [1,
 *            19]
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic programming with O(19*n) time; memoization not needed but
       a static array is reused to avoid repeated allocations. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Allocate or grow the DP array if necessary */
    if (n + 1 > dp_size) {
        seq_t *new_dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_dp)
            return 0; /* allocation failed */
        dp = new_dp;
        dp_size = n + 1;
    }

    /* Initialize the array for this call */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* For each part c in 1..19, add its contribution once (distinct parts)   */
    for (idx_t coin = 1; coin <= 19; ++coin) {
        for (idx_t j = n; j >= coin; --j)
            dp[j] += dp[j - coin];
    }

    return dp[n];
}

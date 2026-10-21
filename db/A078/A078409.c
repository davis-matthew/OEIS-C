/*
 * A078409  Number of ways to partition 4*n+1 into distinct positive integers.
 * https://oeis.org/A078409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 4*n+1 into distinct positive
 *            integers. Computed by dynamic programming over parts: dp[s] =
 *            number of ways to get sum s using parts up to current part. The
 *            array is recomputed from scratch whenever the needed maximum sum
 *            increases. This gives O(maxN²) precomputation...
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sum that must be partitioned. */
    idx_t target = 4 * n + 1;

    /* Static state for the dynamic‑programming table. */
    static seq_t *dp = NULL;             /* dp[s] for 0 ≤ s ≤ current_max */
    static idx_t current_max = 0;        /* highest index already computed */

    /* Need to recompute for a larger target? */
    if (target > current_max)
    {
        /* Allocate or grow the table to hold indices up to target. */
        size_t new_size = (size_t)(target + 1);

        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        dp = new_dp;

        /* Initialise all entries to zero. */
        for (idx_t i = 0; i <= target; ++i)
            dp[i] = 0;

        /* Base case: one way to make 0 (empty partition). */
        dp[0] = 1;

        /* Classic distinct‑parts DP. */
        for (idx_t part = 1; part <= target; ++part)
        {
            for (idx_t s = target; ; --s)   /* iterate downwards */
            {
                dp[s] += dp[s - part];
                if (s == part) break;       /* avoid underflow of unsigned */
            }
        }

        /* Record the new maximum. */
        current_max = target;
    }

    /* Return the answer for the requested sum. */
    return dp[target];
}

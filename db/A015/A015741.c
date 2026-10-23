/*
 * A015741  Number of 6's in all the partitions of n into distinct parts.
 * https://oeis.org/A015741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 89, u32 n <= 253, u64 n <= 817,
 *            u128 n <= 2866, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015741:  Number of 6's in all partitions of n into distinct parts.
   The number equals the number of partitions of n-6 into distinct parts that
   do NOT use the part 6 (since each such partition contributes exactly one 6).
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* For n < 6 no 6 can appear in any partition */
    if (n < 6) return 0;

    /* We need to know partitions of n-6 into distinct parts without using 6 */
    const idx_t target = n - 6;

    /* Static cache for dynamic programming array */
    static seq_t *dp = NULL;   /* dp[i] = number of partitions of i into distinct parts,
                                 avoiding the part 6. */
    static idx_t dp_n = 0;     /* size of the currently cached dp array minus 1 */

    /* Recompute if we need a larger dp array */
    if (dp == NULL || target > dp_n)
    {
        idx_t new_n = target;                       /* New maximum index needed */

        /* Allocate new array */
        seq_t *new_dp = alloc_seq_array(new_n + 1);

        /* Base case: one way to partition 0 (empty partition) */
        new_dp[0] = 1;

        /* Standard DP for distinct parts, skipping part 6 */
        for (idx_t part = 1; part <= new_n; ++part)
        {
            if (part == 6) continue;                /* Skip the forbidden part */

            for (idx_t sum = new_n; sum >= part; --sum)
            {
                new_dp[sum] += new_dp[sum - part];
            }
        }

        /* Replace the cached array (leak is harmless for this context) */
        dp   = new_dp;
        dp_n = new_n;
    }

    /* The desired count is dp[target] */
    return dp[target];
}

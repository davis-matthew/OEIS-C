/*
 * A029062  Expansion of 1/((1-x)*(1-x^3)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 11 and 12
 * Range:     u8 n <= 71, u16 n <= 524, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    /* Parts allowed in the partitions */
    static const idx_t parts[4] = {1, 3, 11, 12};

    static seq_t *dp = NULL;           /* cache of computed values      */
    static idx_t  max_computed = 0;    /* highest index fully computed */

    /* First call or request of a larger index: recompute from scratch. */
    if (dp == NULL || n > max_computed) {
        /* Allocate storage for (n + 1) entries, initialized to 0. */
        seq_t *new_dp = alloc_seq(n + 1);
        for (idx_t i = 0; i <= n; ++i)
            new_dp[i] = 0;

        /* Base case: empty partition. */
        new_dp[0] = 1;

        /* Dynamic‑programming: for each part, add its contributions. */
        for (int pi = 0; pi < 4; ++pi) {
            idx_t p = parts[pi];
            for (idx_t i = p; i <= n; ++i) {
                new_dp[i] += new_dp[i - p];
            }
        }

        /* Replace old cache. */
        free(dp);
        dp = new_dp;
        max_computed = n;
    }

    return dp[n];
}

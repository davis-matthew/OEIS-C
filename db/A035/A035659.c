/*
 * A035659  Number of partitions of n into parts 7k+1 and 7k+4 with at least
 *          one part of each type.
 * https://oeis.org/A035659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 127, u32 n <= 394, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over partitions using parts of form
   7k+1 and 7k+4, counting partitions that contain at least one part of
   each residue type. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* index 1 is the first term of OEIS; values for n < 1 are unused. */
    if (n == 0)
        return (seq_t)0;

    /* static state to build the DP table only once (or up to the
       currently requested n). */
    static seq_t *dp = NULL;
    static idx_t dp_max = 0;

    if (n > dp_max)
    {
        /* Allocate space for (n+1) * 4 DP states. */
        size_t dp_size = (size_t)(n + 1) * 4;
        seq_t *new_dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
        if (!new_dp)
            return (seq_t)0;          /* allocation failure → 0 (should not happen). */
        free(dp);
        dp = new_dp;

        /* Base case: one way to partition 0 with no parts. */
        dp[0 * 4 + 0] = (seq_t)1;

        /* Process every allowed part value in increasing order. */
        for (idx_t val = 1; val <= n; ++val)
        {
            int mod = (int)(val % 7);
            if (mod == 1 || mod == 4)
            {
                int type_bit = (mod == 1) ? 1 : 2;   /* bit for mask 1 or 2. */

                /* Standard unbounded knapsack DP: for each part we
                   consider all totals i from val to n. */
                for (idx_t i = val; i <= n; ++i)
                {
                    idx_t src = (i - val) * 4;
                    idx_t dst = i * 4;

                    /* Existing partitions of i-val with every mask. */
                    for (int mask = 0; mask < 4; ++mask)
                    {
                        seq_t cnt = dp[src + mask];
                        if (cnt)
                        {
                            int new_mask = mask | type_bit;
                            dp[dst + new_mask] += cnt;
                        }
                    }
                }
            }
        }

        dp_max = n;
    }

    /* dp[n][3] gives partitions with both types present. */
    return dp[n * 4 + 3];
}

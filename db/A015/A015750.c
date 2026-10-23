/*
 * A015750  Number of partitions of n into distinct parts, none being 5.
 * https://oeis.org/A015750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts, none being
 *            5. Computed by dynamic programming on the set of admissible
 *            parts {1,2,3,4,6,7,…}
 * Range:     u8 n <= 31, u16 n <= 83, u32 n <= 247, u64 n <= 811,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* dp[i] = number of partitions of i with distinct parts ≠5 */
static idx_t max_n = (idx_t)-1;   /* highest n for which dp is valid */

seq_t a(idx_t n)
{
    /* Recompute table if a larger n is requested */
    if (dp == NULL || n > max_n)
    {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (new_dp == NULL) exit(EXIT_FAILURE);
        dp = new_dp;

        /* initialise */
        for (idx_t i = 0; i < new_size; ++i)
            dp[i] = 0;
        dp[0] = 1;   /* empty partition */

        /* dynamic programming over admissible parts */
        for (idx_t p = 1; p < new_size; ++p)
        {
            if (p == 5) continue;   /* 5 is forbidden */
            for (idx_t i = new_size - 1; i >= p; --i)
                dp[i] += dp[i - p];
        }
        max_n = n;
    }
    return dp[n];
}

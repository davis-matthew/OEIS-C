/*
 * A036029  Number of partitions of n into parts not of form 4k+2, 24k, 24k+1
 *          or 24k-1. Also number of partitions in which no odd part is
 *          repeated, with no part of size less than or equal to 2 and where
 *          differences between parts at distance 5 are greater than 1 when
 *          the smallest part is odd and greater than...
 * https://oeis.org/A036029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts p where * p % 4 != 2
 *            and p % 24 ∉ {0, 1, 23}
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* dp[i] holds a(i) for i = 0..dp_size-1 */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* size of allocated array */

    if (n == 0)                  /* offset starts at 1 */
        return 0;

    if (n >= dp_size) {          /* need to recompute up to n */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
        if (!new_dp)              /* allocation failure → 0 */
            return 0;

        new_dp[0] = 1;           /* base case: one way to partition 0 */

        /* dynamic programming over allowed part sizes */
        for (idx_t part = 3; part <= n; ++part) {
            /* skip disallowed residues: 4k+2 */
            if (part % 4 == 2)
                continue;
            idx_t r24 = part % 24;
            /* skip residues 0, 1, 23 modulo 24 */
            if (r24 == 0 || r24 == 1 || r24 == 23)
                continue;

            for (idx_t sum = part; sum <= n; ++sum)
                new_dp[sum] += new_dp[sum - part];
        }

        free(dp);                /* discard old table */
        dp = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}

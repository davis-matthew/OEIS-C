/*
 * A008675  Expansion of 1/( Product_{j=0..5} (1-x^(2*j+1)) ).
 * https://oeis.org/A008675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 5, 7, 9, 11
 * Range:     u8 n <= 33, u16 n <= 134, u32 n <= 1380, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    /* list of allowed parts */
    const idx_t parts[6] = {1, 3, 5, 7, 9, 11};

    /* allocate or reallocate the cache if necessary */
    if (dp == NULL || n > max_n)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *newdp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!newdp)
            return 0;                 /* allocation failed */
        dp = newdp;

        /* initialise all entries to 0 and set base case */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* dynamic programming: count partitions using each part */
        for (int p = 0; p < 6; ++p)
        {
            idx_t s = parts[p];
            for (idx_t j = s; j <= n; ++j)
                dp[j] += dp[j - s];
        }
        max_n = n;
    }

    /* if n <= max_n, value already computed */
    return dp[n];
}

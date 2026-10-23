/*
 * A029140  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^4)*(1-x^10)).
 * https://oeis.org/A029140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 4 and 10
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values. */
    static seq_t *dp = NULL;
    static idx_t cache_size = 0;      /* number of elements in dp (== current_max+1) */

    /* Allocate or resize the cache to accommodate index n. */
    if (n + 1 > cache_size) {
        /* Allocate or grow the array.  Use an explicit cast for C++ compatibility. */
        seq_t *newdp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        if (!newdp) return 0;          /* allocation failure – unlikely */

        /* Zero the new array and set the base case. */
        for (idx_t i = 0; i <= n; ++i)
            newdp[i] = 0;
        newdp[0] = 1;

        /* Allowed part sizes. */
        const idx_t parts[4] = {2, 3, 4, 10};

        /* Standard DP: for each part, add its contributions to all larger indices. */
        for (int p = 0; p < 4; ++p) {
            idx_t part = parts[p];
            for (idx_t i = part; i <= n; ++i)
                newdp[i] += newdp[i - part];
        }

        dp = newdp;
        cache_size = n + 1;
    }

    return dp[n];
}

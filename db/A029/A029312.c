/*
 * A029312  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts 3, 7, 9, and 12
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *dp = NULL;        /* array of coefficients */
    static idx_t dp_size = 0;       /* current allocated size */

    /* If requested n exceeds current cache, re‑allocate and recompute */
    if (n + 1 > dp_size)
    {
        /* Allocate new array, zero‑initialized */
        seq_t *new_dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!new_dp)
            return 0;   /* memory failure fallback */

        /* Free old cache */
        free(dp);
        dp = new_dp;
        dp_size = n + 1;

        /* Standard coin‑change DP: unlimited parts 3, 7, 9, 12 */
        dp[0] = (seq_t)1;
        static const idx_t parts[] = {3, 7, 9, 12};
        const size_t num_parts = sizeof(parts) / sizeof(parts[0]);

        for (size_t pi = 0; pi < num_parts; ++pi)
        {
            idx_t p = parts[pi];
            for (idx_t i = p; i <= n; ++i)
                dp[i] += dp[i - p];
        }
    }
    return dp[n];
}

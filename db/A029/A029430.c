/*
 * A029430  Expansion of 1/((1-x^6)(1-x^9)(1-x^11)(1-x^12)).
 * https://oeis.org/A029430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 6, 9, 11, and 12
   (unbounded coin–change DP). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of computed values up to the largest n seen so far */
    static seq_t *dp = NULL;
    static idx_t dp_len = 0;            /* actual allocated size    */

    if (n >= dp_len) {
        /* Allocate a new array of length n+1 */
        idx_t new_len = n + 1;
        seq_t *newdp = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        if (!newdp) return 0;           /* allocation failure */

        /* Initialise : one way to make 0, none otherwise */
        newdp[0] = 1;
        for (idx_t i = 1; i < new_len; ++i) newdp[i] = 0;

        /* Parts allowed in the partitions */
        static const idx_t parts[4] = {6, 9, 11, 12};

        /* Unbounded knapsack (coin–change) DP */
        for (int pidx = 0; pidx < 4; ++pidx) {
            idx_t p = parts[pidx];
            for (idx_t i = p; i <= n; ++i) {
                newdp[i] = newdp[i] + newdp[i - p];
            }
        }

        /* Replace old cache */
        free(dp);
        dp     = newdp;
        dp_len = new_len;
    }

    return dp[n];
}

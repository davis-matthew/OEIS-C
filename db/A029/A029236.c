/*
 * A029236  Expansion of 1/((1-x^2)*(1-x^8)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 809, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of partitions of n into parts 2, 8, 9, 10
   by dynamic programming. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for increasing n */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Reallocate and recompute if we need a larger array */
    if (n >= dp_size) {
        idx_t new_size = n + 1U;

        /* Allocate new array */
#ifdef __cplusplus
        seq_t *new_dp = new seq_t[new_size];
        /* (new_dp) is default‑constructed to zero for all elements */
#else
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp) abort(); /* allocation failure */
        /* Zero‑initialize the new array */
        memset(new_dp, 0, new_size * sizeof(seq_t));
#endif

        /* DP parts */
        const idx_t parts[4] = { 2, 8, 9, 10 };

        /* Start with a single part of size 0 */
        new_dp[0] = 1;

        /* Compute partitions for all sizes up to n */
        for (int p = 0; p < 4; ++p) {
            idx_t part = parts[p];
            if (part > new_size) continue; /* skip if part too large */
            for (idx_t i = part; i < new_size; ++i) {
                new_dp[i] += new_dp[i - part];
            }
        }

        /* Free old cache */
        if (dp) {
#ifdef __cplusplus
            delete[] dp;
#else
            free(dp);
#endif
        }

        dp = new_dp;
        dp_size = new_size;
    }

    /* Return the cached value */
    return dp[n];
}

/*
 * A035637  Number of partitions of n into parts 6k and 6k+1 with at least one
 *          part of each type.
 * https://oeis.org/A035637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 123, u32 n <= 368, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035637: Number of partitions of n into parts 6k and 6k+1
   with at least one part of each type.
   The formula used here is:
      a(n) = p_union(n) – p_6k(n) – p_{6k+1}(n)
   where p_union counts partitions using any of the two part types,
   p_6k counts partitions using only multiples of 6,
   and p_{6k+1} counts partitions using only parts congruent to 1 modulo 6.
   This is a direct dynamic‑programming implementation of the generating
   functions and satisfies the required interface and performance. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static storage for memoisation of the largest computed n. */
    static seq_t *dp_all = NULL;   /* partitions using all allowed parts   */
    static seq_t *dpA   = NULL;    /* partitions using only 6k parts      */
    static seq_t *dpB   = NULL;    /* partitions using only 6k+1 parts    */
    static idx_t  current_max = 0; /* largest n for which tables are valid */

    if (n < 1) /* sequence offset is 1 */
        return (seq_t)0;

    /* Extend tables if a larger n is requested. */
    if (n > current_max)
    {
        /* Allocate new tables and initialise them to zero. */
        seq_t *na = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        seq_t *nb = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        seq_t *nc = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));

        /* Free old tables. */
        free(dp_all);
        free(dpA);
        free(dpB);

        dp_all = na;
        dpA   = nb;
        dpB   = nc;
        current_max = n;

        /* Base case: one way to partition 0 (empty partition). */
        dp_all[0] = (seq_t)1;
        dpA[0]   = (seq_t)1;
        dpB[0]   = (seq_t)1;

        /* Compute dp_all: use all parts 6k or 6k+1. */
        for (idx_t p = 1; p <= n; ++p)
        {
            if ((p % 6 == 0) || (p % 6 == 1))
            {
                for (idx_t s = p; s <= n; ++s)
                {
                    dp_all[s] += dp_all[s - p];
                }
            }
        }

        /* Compute dpA: only multiples of 6. */
        for (idx_t p = 6; p <= n; p += 6)
        {
            for (idx_t s = p; s <= n; ++s)
            {
                dpA[s] += dpA[s - p];
            }
        }

        /* Compute dpB: only parts ≡ 1 (mod 6). */
        for (idx_t p = 1; p <= n; p += 6)
        {
            for (idx_t s = p; s <= n; ++s)
            {
                dpB[s] += dpB[s - p];
            }
        }
    }

    /* Return partitions that use at least one part of each type. */
    return dp_all[n] - dpA[n] - dpB[n];
}

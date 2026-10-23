/*
 * A035374  Number of partitions of n into parts 5k+3 or 5k+4.
 * https://oeis.org/A035374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035374: Number of partitions of n into parts 5k+3 or 5k+4.
   Computes the values by dynamic programming, storing all partition
   counts up to the maximum n requested so far.  On each call,
   if the cache already contains the requested n, the stored value
   is returned; otherwise the cache is enlarged and all counts up
   to n are recomputed.  The algorithm runs in O(n²/10) time and
   uses only integer arithmetic. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *cache = NULL;   /* memoised partition counts */
static idx_t max_cached = 0;  /* largest n for which cache has been computed */

/* Return the number of partitions of n into parts 5k+3 or 5k+4. */
seq_t a(idx_t n)
{
    if (n < 1) /* undefined for n < 1, but avoid UB */
        return 0;

    /* Return cached value if already available */
    if (n <= max_cached && cache != NULL)
        return cache[n];

    /* Allocate a new cache big enough for the requested n */
    idx_t new_max = n;
    seq_t *new_cache = alloc_seq(new_max + 1);

    /* Initialize to zero, except for the empty partition */
    for (idx_t i = 0; i <= new_max; ++i)
        new_cache[i] = 0;
    new_cache[0] = 1;

    /* For each allowed part size 5k+3 or 5k+4, update the DP table. */
    for (idx_t k = 0; ; ++k) {
        idx_t p3 = 5 * k + 3;
        if (p3 > new_max)
            break;

        for (idx_t i = p3; i <= new_max; ++i)
            new_cache[i] += new_cache[i - p3];

        idx_t p4 = 5 * k + 4;
        if (p4 > new_max)
            continue;

        for (idx_t i = p4; i <= new_max; ++i)
            new_cache[i] += new_cache[i - p4];
    }

    /* Replace the old cache with the new one */
    if (cache != NULL)
        free_seq(cache);
    cache = new_cache;
    max_cached = new_max;

    return cache[n];
}

/*
 * A025880  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A025880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 399, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A025880
 *  Partitions of n into parts 5, 6, and 11.
 *  Recurrence:  a(0)=1, a(n)=a(n-5)+a(n-6)+a(n-11)  (n≥1, terms
 *  for negative indices are 0).  The implementation uses a
 *  dynamic programming approach that stores all values up to the
 *  largest n requested so far.  When a larger value is requested the
 *  whole table is recomputed from scratch, which is fast enough for
 *  n up to 10000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached partition numbers */
    static size_t cache_size = 0;  /* number of elements stored */

    /* We need indices 0 .. n, thus array of size n+1 */
    size_t needed = (size_t)n + 1;

    if (needed <= cache_size) {
        /* already computed */
        return cache[n];
    }

    /* Allocate new array and compute from scratch up to `n` */
    seq_t *newcache = alloc_seq(needed);
    if (!newcache) {
        return (seq_t)0;   /* allocation failure */
    }

    /* initialise to zero */
    for (size_t i = 0; i < needed; ++i) {
        newcache[i] = (seq_t)0;
    }
    /* base case */
    if (needed > 0) {
        newcache[0] = (seq_t)1;
    }

    /* Parts allowed */
    static const idx_t parts[3] = {5, 6, 11};

    /* Dynamic programming: for each part, accumulate contributions */
    for (size_t pi = 0; pi < 3; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i < (idx_t)needed; ++i) {
            newcache[i] += newcache[i - p];
        }
    }

    /* Replace old cache */
    if (cache) {
        free(cache);
    }
    cache = newcache;
    cache_size = needed;

    return cache[n];
}

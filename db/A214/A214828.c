/*
 * A214828  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 1, a(1) = a(2) = 6.
 * https://oeis.org/A214828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 1, a(1) = a(2) = 6
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* use provided allocator and memory‑growth helper */
#include "../utils/alloc_seq_array.h"
#include "../utils/ensure_capacity.h"

/* Compute the Ternary Fibonacci sequence A214828.
   This function is thread‑unsafe but has static memoisation: all
   previously computed terms are stored for reuse in subsequent calls. */
seq_t a(idx_t n)
{
    /* static cache: array of previously computed values      */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;      /* allocated capacity                */
    static idx_t    sz    = 0;      /* number of valid entries in cache  */

    /* initialise the cache with the first three terms if necessary */
    if (cache == NULL) {
        cache = alloc_seq_array(3);   /* allocate 3 slots          */
        cap   = 3;
        sz    = 3;
        cache[0] = 1;                /* a(0) */
        cache[1] = 6;                /* a(1) */
        cache[2] = 6;                /* a(2) */
    }

    /* if the requested index is already computed, return it */
    if (n < sz)
        return cache[n];

    /* grow the allocated buffer if required */
    if ((size_t)n + 1 > cap)
        ensure_capacity((size_t)n + 1, &cap,
                        (void **)&cache, sizeof(seq_t));

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = sz; i <= n; ++i)
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];

    sz = (idx_t)n + 1; /* update the number of stored terms       */
    return cache[n];
}

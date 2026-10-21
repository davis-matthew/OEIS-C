/*
 * A223212  3X3X3 triangular graph coloring a rectangular array: number of nX2
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,2
 *          1,2 1,3 1,4 2,4 3,4 2,5 4,5 and every array movement to a
 *          horizontal or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 9*a(n-2) a(1) = 18, a(2) = 126
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"   /* allocates an array of seq_t */
#include "../utils/seq_realloc.h" /* reallocates an existing array of seq_t */

seq_t a(idx_t n)
{
    /* undefined for n < 1, return 0 in that case */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache      = NULL;   /* array of cached sequence values */
    static size_t cache_cap  = 0;       /* current capacity of cache          */
    static idx_t  max_cached = 0;       /* highest n that has been computed   */

    /* Initialise cache on first call */
    if (!cache)
    {
        /* capacity 3: indexes 0,1,2 are used (0 is unused) */
        cache     = seq_alloc(3);
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)18;
        cache[2]  = (seq_t)126;
        cache_cap = 3;
        max_cached = 2;
    }

    /* If the requested term has already been computed, return it */
    if (n <= max_cached)
        return cache[(size_t)n];

    /* Grow the cache to hold values up to n (inclusive) */
    if (n + 1 > cache_cap)
    {
        seq_t *tmp = seq_realloc(cache, cache_cap, n + 1);
        if (!tmp)          /* allocation failure – give up */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = n + 1;
    }

    /* Compute all missing terms using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        cache[(size_t)i] = (seq_t)6 * cache[(size_t)i - 1]
                         + (seq_t)9 * cache[(size_t)i - 2];
    }

    max_cached = n;
    return cache[(size_t)n];
}

/*
 * A232921  Number of 2 X n 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2), with a(1)=9, a(2)=18
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for values already computed */
    static seq_t *cache   = NULL;
    static idx_t  cache_n = 0;   /* highest index computed (>=1) */
    static size_t  cache_cap = 0;/* allocated number of elements (>=n+1) */

    /* allocate or grow the cache to reach n+1 entries (index 0 unused) */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n) + 2;          /* include index 0 and one spare */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failure (unlikely) */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise first two known terms if not yet done */
    if (cache_n < 1) {
        cache[1] = (seq_t)9;
        cache_n  = 1;
    }
    if (cache_n < 2) {
        cache[2] = (seq_t)18;
        cache_n  = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-1] + cache[i-2] + cache[i-2];
    }
    cache_n = n;

    return cache[n];
}

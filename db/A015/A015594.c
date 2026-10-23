/*
 * A015594  a(n) = 11*a(n-1) + 3*a(n-2).
 * https://oeis.org/A015594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; a(n) = 11*a(n-1) + 3*a(n-2) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (O(n) fill) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t sz    = 0;   /* number of valid elements */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        sz = 2;
    }

    /* if the requested index is already cached */
    if (n < sz)
        return cache[n];

    /* otherwise grow the cache to at least n+1 */
    if (n >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)11 * cache[i - 1] + (seq_t)3 * cache[i - 2];
    }
    sz = (size_t)n + 1;

    return cache[n];
}

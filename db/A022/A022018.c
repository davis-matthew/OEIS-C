/*
 * A022018  Define the sequence UD(a(0),a(1)) by a(n) is the least integer
 *          such that a(n)/a(n-1) > a(n-1)/a(n-2)+1 for even n >= 2 and such
 *          that a(n)/a(n-1) > a(n-1)/a(n-2) for odd n>=2. This is UD(2,16).
 * https://oeis.org/A022018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=16, a(2)=129, a(n)=8*a(n-1)+a(n-2)-4*a(n-3) for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the n-th term of OEIS sequence A022018.
 * The function is thread‑unsafe but satisfies the required
 * non‑modifying interface.  It lazily allocates a cache and
 * grows it as needed.
 */
seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cache_size = 0; /* number of cached terms        */

    /* initialise the cache with the first three terms */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* out of memory, return 0 as a sentinel */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)129;
    }

    /* if n is already cached, return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* extend the cache up to index n */
    size_t old_size = cache_size;
    cache_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp) return 0;               /* allocation failure – same sentinel */
    cache = tmp;

    /* compute missing terms */
    for (size_t i = old_size; i <= n; ++i) {
        cache[i] = (seq_t)8 * cache[i-1] + cache[i-2]
                 - (seq_t)4 * cache[i-3];
    }

    return cache[n];
}

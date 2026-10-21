/*
 * A052925  Expansion of (2-6*x+4*x^2-x^3)/((1-x)*(1-3*x+x^2)).
 * https://oeis.org/A052925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) with a(0)=2, a(1)=2, a(2)=4,
 *            a(3)=9
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* Initialise the cache with the first four terms on the first call */
    if (cache == NULL) {
        cache_cap   = 8;                   /* initial allocation */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out‑of‑memory fallback */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache_size = 4;                    /* indices 0..3 are valid */
    }

    size_t idx = (size_t)n;

    /* Return the term if it has already been computed */
    if (idx < cache_size)
        return cache[idx];

    /* Grow the cache so it can hold the requested index */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (idx + 1 > new_cap) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* out‑of‑memory fallback */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute the missing terms sequentially up to idx */
    for (size_t i = cache_size; i <= idx; ++i) {
        seq_t val = 4 * cache[i-1] - 4 * cache[i-2] + cache[i-3];
        cache[i] = val;
    }
    cache_size = idx + 1;

    return cache[idx];
}

/*
 * A042014  Numerators of continued fraction convergents to sqrt(531).
 * https://oeis.org/A042014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1060*a(n-2) - a(n-4), with * initial values a(0)=23,
 *            a(1)=530, a(2)=24403, a(3)=561799
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External integer types:
 *   seq_t   – the sequence element type (uint8_t … uint128_t or big integer)
 *   idx_t   – 64‑bit unsigned integer (uint64_t)
 */
seq_t a(idx_t n)
{
    /* Lazy static cache of computed values */
    static seq_t *cache = NULL;      /* pointer to allocated array */
    static size_t cache_len = 0;     /* number of elements already set */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* If first call, allocate initial cache and fill base cases */
    if (cache_len == 0) {
        cache_cap = 8;  /* initial capacity, large enough for first few terms */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – return 0 as fallback */

        /* base values a(0) … a(3) */
        cache[0] = (seq_t)23;
        cache[1] = (seq_t)530;
        cache[2] = (seq_t)24403;
        cache[3] = (seq_t)561799;
        cache_len = 4;
    }

    /* If requested index is already cached, return it */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Ensure sufficient capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 as fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Recurrence uses a(i-2) and a(i-4); i>=4 guaranteed */
        cache[i] = (seq_t)1060 * cache[i - 2] - cache[i - 4];
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

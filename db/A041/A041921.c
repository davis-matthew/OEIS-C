/*
 * A041921  Denominators of continued fraction convergents to sqrt(482).
 * https://oeis.org/A041921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 966*a(n-4) - a(n-8) for n>7 with initial values a(0)=1,
 *            a(1)=1, a(2)=21, a(3)=22, a(4)=945, a(5)=967, a(6)=20285,
 *            a(7)=21252
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;      /* allocated array size          */
    static size_t len      = 0;      /* number of terms stored so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 16;                       /* start with space for at least 8 terms */
        cache    = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out‑of‑memory */

        /* initial known values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)945;
        cache[5] = (seq_t)967;
        cache[6] = (seq_t)20285;
        cache[7] = (seq_t)21252;

        len = 8;   /* first eight terms are now cached */
    }

    /* If the requested index is already cached, return it */
    if ((size_t)n < len)
        return cache[(size_t)n];

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;  /* double up */
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;         /* out‑of‑memory */
        memcpy(new_cache, cache, len * sizeof(seq_t));
        free(cache);
        cache    = new_cache;
        capacity = new_cap;
    }

    /* Compute missing terms up to n inclusive */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)966 * cache[i-4] - cache[i-8];
        cache[i] = term;
    }

    /* Update the length of cached data */
    if (len < (size_t)n + 1)
        len = (size_t)n + 1;

    return cache[(size_t)n];
}

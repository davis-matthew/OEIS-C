/*
 * A092900  A Jacobsthal sequence (A078008) to base 4.
 * https://oeis.org/A092900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 11*a(n-2) - 10*a(n-4) for n>3 a(0) = 1, a(1) = 0, a(2) =
 *            2, a(3) = 2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;  /* array of computed terms */
    static idx_t  cache_size = 0;     /* number of terms stored */
    static size_t cache_cap  = 0;     /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap   = 4;
        cache_size  = 4;
        cache       = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;                /* unlikely, but safety */

        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        cache[3] = (seq_t)2;   /* a(3) */
    }

    /* Return immediately if the term is already cached */
    if (n < (idx_t)cache_size)
        return cache[(size_t)n];

    /* Grow cache and compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Ensure there is room for the new element */
        if ((size_t)i >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 4; /* initial protection */
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;     /* allocation failure */
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* recurrence a(i) = 11*a(i-2) - 10*a(i-4) */
        seq_t term = (cache[i-2] * (seq_t)11) - (cache[i-4] * (seq_t)10);
        cache[i]   = term;
    }

    cache_size = n + 1;
    return cache[(size_t)n];
}

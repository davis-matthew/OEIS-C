/*
 * A228579  x-values in the solution to the Pell equation x^2 - 106*y^2 = -1.
 * https://oeis.org/A228579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228579
   Recurrence: a(n) = 64160102*a(n-1) - a(n-2)
   Initial values: a(1)=4005, a(2)=256961212515
   Offset: 1 (a(1) is the first term). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* guard for the official offset */
    if (n < 1)
        return (seq_t)0;

    /* recurrence multiplier (constant fits in any supported seq_t) */
    const seq_t MULT = (seq_t)64160102;

    /* static cache that grows as needed */
    static seq_t *cache = NULL;    /* array of computed terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of valid terms */

    /* initialise the cache on first call */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)4005;                    /* a(1) */
        cache[1] = (seq_t)256961212515ULL;         /* a(2) */
        cache_len = 2;
    }

    /* extend the cache until it contains at least n terms */
    while (cache_len < n) {
        /* double capacity when necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute next term using the recurrence */
        cache[cache_len] = MULT * cache[cache_len - 1] - cache[cache_len - 2];
        ++cache_len;
    }

    return cache[n - 1];
}

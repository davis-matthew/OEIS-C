/*
 * A239150  Number of (n+1) X (1+1) 0..3 arrays with no element equal to all
 *          horizontal neighbors or unequal to all vertical neighbors, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A239150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 7*a(n-2) - 26*a(n-3) + 5*a(n-4) + 14*a(n-5)
 *            with a(1)=1, a(2)=1, a(3)=7, a(4)=17, a(5)=96
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms, indexed from 0.. */
    static seq_t *cache   = NULL;   /* array of cached values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_upto = 0;    /* highest index that has been computed (≥0) */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with a modest capacity, but at least 6 for the initial values */
        size_t init_cap = 16;
        if (init_cap < 6) init_cap = 6;
        seq_t *tmp = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        cache = tmp;
        cache_cap = init_cap;

        /* store the first five terms; cache[0] is unused but set to 0 for safety */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)96;
        cache_upto = 5;
    }

    /* grow the cache if the requested index is beyond the current capacity */
    size_t needed = (size_t)n + 1;        /* indices 0 .. n must exist */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if we already have the requested value, return it immediately */
    if (n <= cache_upto)
        return cache[(size_t)n];

    /* compute missing terms up to n using the recurrence */
    size_t i;
    for (i = cache_upto + 1; i <= (size_t)n; ++i) {
        seq_t term = cache[i-1] * (seq_t)4
                   + cache[i-2] * (seq_t)7
                   - cache[i-3] * (seq_t)26
                   + cache[i-4] * (seq_t)5
                   + cache[i-5] * (seq_t)14;
        cache[i] = term;
    }

    cache_upto = n;
    return cache[(size_t)n];
}

/*
 * A042498  Numerators of continued fraction convergents to sqrt(777).
 * https://oeis.org/A042498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 446*a(n-4) - a(n-8),
   a(0)=27, a(1)=28, a(2)=195, a(3)=223,
   a(4)=12237, a(5)=12460, a(6)=86997, a(7)=99457 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_cap = 0;     /* allocated capacity (in terms) */
    static size_t cache_size = 0;    /* number of initialised terms */

    /* initial 8 terms stored as constants */
    static const seq_t init[8] = {
        (seq_t)27,
        (seq_t)28,
        (seq_t)195,
        (seq_t)223,
        (seq_t)12237,
        (seq_t)12460,
        (seq_t)86997,
        (seq_t)99457
    };

    /* initialise the cache on the first call */
    if (cache_size == 0)
    {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_size = 8;
    }

    /* grow the cache if the requested n is beyond the current size */
    if ((idx_t)n >= cache_size)
    {
        size_t new_cap = (size_t)n + 1;          /* need indices 0 .. n */
        if (new_cap > cache_cap)
        {
            seq_t *tmp = (seq_t*)realloc(cache,
                                          new_cap * sizeof(seq_t));
            if (tmp == NULL) /* allocation failed: abort */
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (size_t i = cache_size; i <= (size_t)n; ++i)
        {
            /* the first 8 terms are already present */
            if (i < 8)
                cache[i] = init[i];
            else
                cache[i] = (seq_t)446 * cache[i-4] - cache[i-8];
        }
        cache_size = new_cap;
    }

    return cache[(size_t)n];
}

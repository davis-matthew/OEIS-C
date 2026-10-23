/*
 * A135055  Pentanacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) +
 *          a(n-5) for n>4 and with a(0)=-2, a(1)=-1, a(2)=0, a(3)=1, a(4)=2.
 * https://oeis.org/A135055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5) with a(0) = -2, a(1) =
 *            -1, a(2) = 0, a(3) = 1, a(4) = 2
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 36, i64 n <= 69, i128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated slots   */
    static size_t cache_sz  = 0;      /* number of terms   */

    /* Initial values */
    static const seq_t init[5] = 
        {(seq_t)-2, (seq_t)-1, (seq_t)0, (seq_t)1, (seq_t)2};

    /* Allocate and initialise the cache on first call */
    if (cache == NULL) {
        cache_cap = 5;                      /* capacity at least 5 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)                  /* allocation failed */
            return (seq_t)0;
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        cache_sz = 5;
    }

    /* Return already cached early terms */
    if (n < 5)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    while (cache_sz <= (size_t)n) {

        /* Ensure sufficient capacity for the new term */
        if (cache_sz >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= cache_sz)
                new_cap = cache_sz + 1;      /* at least one slot ahead */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)                /* allocation failure */
                return (seq_t)0;
            cache      = tmp;
            cache_cap  = new_cap;
        }

        seq_t sum = cache[cache_sz-1];
        sum += cache[cache_sz-2];
        sum += cache[cache_sz-3];
        sum += cache[cache_sz-4];
        sum += cache[cache_sz-5];
        cache[cache_sz] = sum;
        ++cache_sz;
    }

    return cache[(size_t)n];
}

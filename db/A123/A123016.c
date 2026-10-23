/*
 * A123016  a(1)=1, a(2)=1, a(3)=4, a(4)=0; a(n)=12a(n-2)-16a(n-3) for n>=5.
 * https://oeis.org/A123016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (16*2^n - (-4)^n)/48, n>0
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 18, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1, a(3)=4, a(4)=0; a(n)=12*a(n-2)-16*a(n-3) for n>=5 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_val = 0;   /* highest index stored in cache */

    /* size needed: indices 1..n inclusive, index 0 unused */
    size_t needed = (size_t)(n + 1);

    /* allocate or grow cache as needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
    } else if (n > cache_val) {
        /* grow cache to accommodate n */
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!cache) abort();
    }

    /* if the requested value is already cached, just return it */
    if (n <= cache_val) {
        return cache[n];
    }

    /* compute missing terms from cache_val+1 up to n */
    for (idx_t i = cache_val + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)1;
        else if (i == 3)
            cache[i] = (seq_t)4;
        else if (i == 4)
            cache[i] = (seq_t)0;
        else
            cache[i] = (seq_t)12 * cache[i - 2] - (seq_t)16 * cache[i - 3];
    }

    cache_val = n;
    return cache[n];
}

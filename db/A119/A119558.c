/*
 * A119558  a(1)=0,a(2)=0,a(3)=1 then a(n)=abs(a(n-1)-a(n-2))+(-1)^n*a(n-3).
 * https://oeis.org/A119558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=0, a(2)=1; for n>=3: a(n)=abs(a(n-1)-a(n-2)) +
 *            (-1)^(n+1)*a(n-3)
 * Range:     i8 n <= 48, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0; /* number of slots allocated      */
    static idx_t cache_max = 0;      /* highest index that has a value */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* we need at least three entries for the base values */
        cache_capacity = (n > 2) ? (n + 1) : 3;
        cache = (seq_t *)realloc(NULL, cache_capacity * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache_max = 2;
    }

    /* grow cache if n is beyond current capacity */
    if (n >= cache_capacity) {
        idx_t new_capacity = cache_capacity ? cache_capacity * 2 : 3;
        if (new_capacity <= n) new_capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* absolute difference of previous two terms */
        seq_t diff = cache[i - 1] - cache[i - 2];
        seq_t absdiff = (diff >= (seq_t)0) ? diff : -diff;

        /* sign factor (-1)^(i+1) */
        seq_t sign = ((i + 1) & 1) ? (seq_t)-1 : (seq_t)1;

        /* recurrence: a(i) = absdiff + sign * a(i-3) */
        cache[i] = absdiff + sign * cache[i - 3];
    }
    cache_max = ((n > cache_max) ? n : cache_max);

    return cache[n];
}

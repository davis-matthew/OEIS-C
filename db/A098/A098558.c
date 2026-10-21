/*
 * A098558  Expansion of e.g.f. (1+x)/(1-x).
 * https://oeis.org/A098558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n! for n >= 1, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache      = NULL;  /* array of a(i) values          */
    static size_t cache_size = 0;      /* allocated size of cache       */
    static idx_t  cached_max = 0;      /* largest n for which cache[i] is valid */
    static seq_t current_fact = (seq_t)1; /* factorial of cached_max    */

    /* a(0) is a special case */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate space for at least index 0 */
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;           /* a(0) */
        current_fact = (seq_t)1;       /* 0! = 1 */
        cached_max = 0;                /* cache valid up to 0 */
    }

    /* if we already have the value, return it */
    if (n <= cached_max)
        return cache[n];

    /* grow cache to hold n */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute factorials and a(i) for i = cached_max+1 .. n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        current_fact = current_fact * (seq_t)i;          /* i!   */
        cache[i]     = current_fact * (seq_t)2;          /* 2*i! */
    }

    cached_max = n;
    return cache[n];
}

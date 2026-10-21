/*
 * A215917  a(n) = -3*a(n-1) + a(n-3), with a(0)=0, a(1)=6, and a(2)=-15.
 * https://oeis.org/A215917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) + a(n-3) , a(0)=0, a(1)=6, a(2)=-15
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 19, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t cache_size = 0;

    /* initialize base values */
    if (!cache) {
        cache_capacity = 3;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failure fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)-15;
        cache_size = 3;
    }

    /* compute up to n if needed */
    if (n >= cache_size) {
        /* grow capacity to accommodate n */
        while (cache_capacity <= n) {
            cache_capacity <<= 1;
            if (cache_capacity == 0) cache_capacity = 1;  /* safety */
        }
        seq_t *tmp = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                     /* allocation failure fallback */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = - ((seq_t)3 * cache[i-1]) + cache[i-3];
        }
        cache_size = n + 1;
    }

    return cache[n];
}

/*
 * A021992  Decimal expansion of 1/988.
 * https://oeis.org/A021992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) + a(n-10) - a(n-9) for n >= 12 * Initial values:
 *            a(0)=0,a(1)=0,a(2)=1,a(3)=0,a(4)=1,a(5)=2,a(6)=1, *
 *            a(7)=4,a(8)=5,a(9)=7,a(10)=4,a(11)=8
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial 12 terms of the sequence */
    static const seq_t init[12] = {
        0, 0, 1, 0, 1, 2, 1, 4, 5, 7, 4, 8
    };

    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently in cache */
    static size_t cache_size = 0;  /* allocated capacity of the cache */

    /* lazily initialize the cache on first call */
    if (!cache) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_len = 12;
    }

    /* extend cache if the requested index is beyond what we have computed */
    if (n >= cache_len) {
        size_t need = (size_t)n + 1;           /* we need indices 0..n inclusive */
        if (need > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
            cache = tmp;
            cache_size = need;
        }

        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence: a[i] = a[i-1] + a[i-10] - a[i-9] */
            cache[i] = cache[i-1] + cache[i-10] - cache[i-9];
        }
        cache_len = need;
    }

    return cache[(size_t)n];
}

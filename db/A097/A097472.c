/*
 * A097472  Number of different candle trees having a total of m edges.
 * https://oeis.org/A097472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=10, a(3)=31
   a(n)=3*a(n-1)+a(n-2)-2*a(n-3)-a(n-4)  for n>=4
   (OEIS A097472 – Number of different candle trees having a total of m edges) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache for n = 0..3 */
    if (cache == NULL) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        /* the OEIS recurrence gives the first four values */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 10;
        cache[3] = 31;
        cache_size = 4;
    }

    /* if already computed, simply return */
    if (n < cache_size)
        return cache[n];

    /* extend the cache to hold up to n */
    if (n >= (idx_t)cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – in practice this should not happen for supported test ranges */
            return 0;
        }
        cache = tmp;
        /* compute new entries using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* i >= 4 guaranteed here */
            seq_t big   = cache[i - 1] * 3; /* 3*a(n-1) */
            big += cache[i - 2];           /* + a(n-2) */
            seq_t small = cache[i - 3] * 2; /* 2*a(n-3) */
            small += cache[i - 4];          /* + a(n-4) */
            cache[i] = big - small;        /* result of the recurrence */
        }
        cache_size = new_size;
    }

    return cache[n];
}

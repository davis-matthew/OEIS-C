/*
 * A041407  Denominators of continued fraction convergents to sqrt(218).
 * https://oeis.org/A041407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 502*a(n-5) + a(n-10) for n > 9 Initial values: a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=13, a(4)=17, a(5)=489, a(6)=506, a(7)=2007,
 *            a(8)=6527, a(9)=8534
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise the cache with the first 10 terms the first time we are called */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(10 * sizeof(seq_t));
        if (!cache) abort();               /* defensive: stop if allocation fails */
        cache[0] = 1; cache[1] = 1; cache[2] = 4;
        cache[3] = 13; cache[4] = 17;
        cache[5] = 489; cache[6] = 506; cache[7] = 2007;
        cache[8] = 6527; cache[9] = 8534;
        cache_len = 10;
    }

    size_t idx = (size_t)n;
    if (idx < cache_len)                /* already computed */
        return cache[idx];

    /* Resize the array to hold up to idx */
    seq_t *tmp = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
    if (!tmp) abort();                  /* stop if reallocation fails */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (size_t i = cache_len; i <= idx; ++i) {
        cache[i] = 502 * cache[i - 5] + cache[i - 10];
    }

    cache_len = idx + 1;
    return cache[idx];
}

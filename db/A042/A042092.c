/*
 * A042092  Numerators of continued fraction convergents to sqrt(570).
 * https://oeis.org/A042092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 382*a(n-4) - a(n-8); Initial values: a(0)=23, a(1)=24,
 *            a(2)=167, a(3)=191, a(4)=8953, a(5)=9144, a(6)=63817, a(7)=72961
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_max = 0;  /* largest n computed (inclusive) */

    /* For n < 8 just return the hard coded values. */
    if (n < 8) {
        static const seq_t init[8] = {23, 24, 167, 191,
                                      8953, 9144, 63817, 72961};
        return init[(size_t)n];
    }

    /* First time: allocate room for at least eight elements and
       fill the initial values. */
    if (cache == NULL) {
        size_t initial = (size_t) (n + 1 > 8 ? n + 1 : 8);
        cache = (seq_t*)malloc(initial * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache_cap = initial;
        cache[0] = 23;  cache[1] = 24; cache[2] = 167; cache[3] = 191;
        cache[4] = 8953; cache[5] = 9144; cache[6] = 63817; cache[7] = 72961;
        cache_max = 7;
    }

    /* Grow the cache if needed */
    if ((size_t)n > cache_cap - 1) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap - 1) new_cap <<= 1;   /* double */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = 382 * cache[i - 4] - cache[i - 8];
    }
    cache_max = n;

    return cache[(size_t)n];
}

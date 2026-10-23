/*
 * A180143  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.: (1 +
 *          x^2)/(1 - 4*x + x^2 + 2*x^3).
 * https://oeis.org/A180143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 2*a(n-3), initial values: a(0)=1,
 *            a(1)=4, a(2)=16
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */
    static idx_t cache_cap = 0;    /* current allocated capacity */

    /* if we already know this value, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* ensure enough space for indices 0 .. n */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    /* initialise the first three terms if the cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)4;
        if (cache_cap > 2) cache[2] = (seq_t)16;
        cache_size = 3;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)4;
        } else if (i == 2) {
            cache[i] = (seq_t)16;
        } else {
            /* recurrence: a(i) = 4*a(i-1) - a(i-2) - 2*a(i-3) */
            cache[i] = 4 * cache[i - 1] - cache[i - 2] - 2 * cache[i - 3];
        }
    }
    cache_size = n + 1;

    return cache[n];
}

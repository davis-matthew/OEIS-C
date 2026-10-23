/*
 * A138112  a(n)=3a(n-1)-4a(n-2)+2a(n-3)-a(n-4), a(0)=a(1)=a(2)=0, a(3)=1,
 *          a(4)=3.
 * https://oeis.org/A138112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n)=3*a(n-1)-4*a(n-2)+2*a(n-3)-a(n-4) with a(0)=a(1)=a(2)=0,
 *            a(3)=1, a(4)=3
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache stores already computed terms up to cache_size-1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms stored */

    /* first invocation: initialise the cache with the first 5 terms */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return 0 as error */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 3;
    }

    /* if requested term already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* need to extend cache to cover n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failed – return 0 as error */
    cache = tmp;

    /* compute new terms from old_size to n inclusive */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t val = 3 * cache[i-1] - 4 * cache[i-2] + 2 * cache[i-3] - cache[i-4];
        cache[i] = val;
    }

    return cache[n];
}

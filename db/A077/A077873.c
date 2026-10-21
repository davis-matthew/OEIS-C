/*
 * A077873  Expansion of (1-x)^(-1)/(1-x+x^2+2*x^3).
 * https://oeis.org/A077873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-3) - 2*a(n-2) + 2*a(n-1) with initial
 *            values a(0)=1, a(1)=2, a(2)=2, a(3)=-1
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 49, i64 n <= 98, i128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache holds computed values from 0 up to the largest n seen */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t target = (size_t)n;

    /* If we already have the value, return it immediately */
    if (target < cache_size)
        return cache[target];

    /* Determine the new cache size (at least 4 to hold the first terms) */
    size_t new_size = target + 1;
    if (new_size < 4) new_size = 4;

    /* Grow the cache array */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;

    /* Initialize the first four terms if this is the first allocation */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)-1;
    }

    /* Compute all missing terms up to n */
    for (size_t i = cache_size; i <= target; ++i) {
        if (i < 4) /* already initialized */ continue;
        seq_t t1 = 2 * cache[i - 4];
        seq_t t2 = cache[i - 3];
        seq_t t3 = 2 * cache[i - 2];
        seq_t t4 = 2 * cache[i - 1];
        cache[i] = t1 - t2 - t3 + t4;
    }

    cache_size = new_size;
    return cache[target];
}

/*
 * A014019  Inverse of 10th cyclotomic polynomial.
 * https://oeis.org/A014019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) for n > 4 Initial
 *            values: a(0)=1, a(1)=1, a(2)=0, a(3)=0, a(4)=0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* capacity of the cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;            /* initial capacity */
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache_len = 5;
    }

    /* if n already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow cache as needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] - cache[i-2] + cache[i-3] - cache[i-4];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}

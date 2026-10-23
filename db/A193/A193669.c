/*
 * A193669  Expansion of o.g.f.(1-x^4)/(1-x+x^8).
 * https://oeis.org/A193669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-8) Initial values: a(0)=a(1)=a(2)=a(3)=1,
 *            a(4)=a(5)=a(6)=a(7)=0
 * Range:     i8 n <= 34, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute A193669 */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed values   */
    static size_t cache_size = 0;    /* number of elements in cache */

    /* helper to initialize first 8 terms */
    if (!cache) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* safety check for allocation failure */
        if (!cache) abort();
        cache[0] = 1;  cache[1] = 1;  cache[2] = 1;  cache[3] = 1;
        cache[4] = 0;  cache[5] = 0;  cache[6] = 0;  cache[7] = 0;
    }

    /* If we already have the value, return it. */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Extend the cache up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();      /* allocation failed */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (size_t i = cache_size; i < new_size; ++i) {
        cache[i] = cache[i-1] - cache[i-8];
    }
    cache_size = new_size;

    return cache[n];
}

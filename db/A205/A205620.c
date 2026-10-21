/*
 * A205620  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A205620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40*a(n-2)+40*a(n-3)-28*a(n-4) for n>5, a(1)=50, a(2)=306,
 *            a(3)=2052, a(4)=13800, a(5)=92934
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t  cache_size = 0;

    /* values for the initial indices */
    static const seq_t init[6] = {
        0,                                 /* unused for index 0 */
        (seq_t)50,
        (seq_t)306,
        (seq_t)2052,
        (seq_t)13800,
        (seq_t)92934
    };

    /* initialize the cache on the first call */
    if (cache == NULL) {
        /* we need indices 0..5 : size 6 */
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */
        memcpy(cache, init, sizeof(init));
    }

    /* if the requested index is already in the cache, return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* enlarge the cache to hold up to index 'n' */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                  /* out of memory */
    cache = tmp;
    size_t old_size = cache_size;
    cache_size = new_size;

    /* compute the missing terms using the recurrence */
    for (size_t i = old_size; i <= (size_t)n; ++i) {
        /* recurrence a(i) = 40*a(i-2)+40*a(i-3)-28*a(i-4) */
        seq_t v1 = cache[i - 2];
        seq_t v2 = cache[i - 3];
        seq_t v3 = cache[i - 4];
        cache[i] = (seq_t)40 * v1 + (seq_t)40 * v2 - (seq_t)28 * v3;
    }

    return cache[(size_t)n];
}

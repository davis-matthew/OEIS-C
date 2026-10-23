/*
 * A167433  Row sums of the Riordan array (1-4*x+4*x^2, x*(1-2*x)) (A167431).
 * https://oeis.org/A167433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = -3, a(2) = -1, a(n) = a(n-1) - 2*a(n-2) for all
 *            n >= 3
 * Range:     i8 n <= 13, i16 n <= 28, i32 n <= 60, i64 n <= 124,
 *            i128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise the cache with the first three terms if it is empty. */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();      /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)-1;
    }

    /* Extend the cache if the requested index is beyond the current size. */
    if ((idx_t)n >= (idx_t)cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        for (size_t i = cache_size; i <= new_size - 1; ++i) {
            if (i >= 3) {
                cache[i] = cache[i-1] - (seq_t)2 * cache[i-2];
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}

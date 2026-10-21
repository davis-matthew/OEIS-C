/*
 * A054768  a(n+2) = (2n+3)*a(n+1) + (n+1)^2*a(n).
 * https://oeis.org/A054768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054768
 * Recurrence: a(0) = -2, a(1) = 1
 * a(n+2) = (2n+3)*a(n+1) + (n+1)^2*a(n)   for n ≥ 0
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: values already computed */
    static seq_t *cache = NULL;
    /* capacity of the allocated array */
    static size_t cache_cap = 0;
    /* number of values currently stored (always ≤ cache_cap) */
    static size_t cache_len = 0;

    /* Ensure enough capacity for indices 0 … n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 2;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        /* Initialise the first two terms if this is the first allocation */
        if (cache_len == 0) {
            cache[0] = (seq_t)-2;
            cache[1] = (seq_t)1;
            cache_len = 2;
        }
    }

    /* Compute missing terms up to n */
    if ((size_t)n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Derive the recurrence parameter k = i-2 */
            idx_t k = i - 2;
            seq_t term = ((seq_t)(2 * k + 3)) * cache[i - 1]
                       + ((seq_t)(k + 1) * (k + 1)) * cache[i - 2];
            cache[i] = term;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}

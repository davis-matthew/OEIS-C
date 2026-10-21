/*
 * A077867  Expansion of (1-x)^(-1)/(1-x-x^2+2*x^3).
 * https://oeis.org/A077867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 41, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 + a(n-1) + a(n-2) - 2*a(n-3),  with a(-1)=a(-2)=a(-3)=0
 * (This is equivalent to the generating function (1-x)^(-1)/(1-x-x^2+2x^3))
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* Initialise cache with the first term a(0) */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        cache_size = 1;
    }

    /* Expand storage if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t a_im1 = (i >= 1) ? cache[i - 1] : (seq_t)0;
        seq_t a_im2 = (i >= 2) ? cache[i - 2] : (seq_t)0;
        seq_t a_im3 = (i >= 3) ? cache[i - 3] : (seq_t)0;
        seq_t val   = (seq_t)1 + a_im1 + a_im2 - ((seq_t)2 * a_im3);
        cache[i]    = val;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}

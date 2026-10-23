/*
 * A158078  G.f. 1/( (1 + x)^7*(1 -7*x +28*x^2 -84*x^3 +210*x^4 -462*x^5
 *          +924*x^6 -1463*x^7 +1738*x^8 -1463*x^9 +924*x^10 -462*x^11
 *          +210*x^12 -84*x^13 +28*x^14 -7*x^15 +x^16) ).
 * https://oeis.org/A158078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0, a(n) = - (253*a(n-7) + 506*a(n-8) +
 *            1288*a(n-11) + 1288*a(n-12) + 506*a(n-15) + 253*a(n-16) +
 *            a(n-23))
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients come from the denominator of the generating
   function 1/(1 + 253*x^7 + 506*x^8 + 1288*x^11 + 1288*x^12
           + 506*x^15 + 253*x^16 + x^23) */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed a(n) */
static idx_t cache_len = 0;   /* number of elements stored (cache_len-1 is last valid index) */

/* Ensure that a[0..n] are computed and stored in cache. */
static void ensure_cached(idx_t n)
{
    if (n < cache_len)
        return;            /* already computed */

    /* Allocate or grow the cache to hold up to index n. */
    size_t new_len = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache == NULL)
            abort();      /* OOM */
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
    }

    /* Initialise base case if we are starting from scratch. */
    if (cache_len == 0) {
        cache[0] = 1;       /* a(0) = 1 */
        cache_len = 1;
    }

    /* Compute any missing terms. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;
        if (i >= 7)  sum += (seq_t)253 * cache[i - 7];
        if (i >= 8)  sum += (seq_t)506 * cache[i - 8];
        if (i >= 11) sum += (seq_t)1288 * cache[i - 11];
        if (i >= 12) sum += (seq_t)1288 * cache[i - 12];
        if (i >= 15) sum += (seq_t)506 * cache[i - 15];
        if (i >= 16) sum += (seq_t)253 * cache[i - 16];
        if (i >= 23) sum +=      cache[i - 23];
        cache[i] = -sum;
    }

    cache_len = n + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cached(n);
    return cache[n];
}

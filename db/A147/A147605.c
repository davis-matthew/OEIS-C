/*
 * A147605  Expansion of g.f.: 1/((1 - x^2 - x^3 - x^4 - x^5 - x^6 - x^7)*(1 +
 *          x + x^2 + x^3 + x^4 + x^5 - x^7)).
 * https://oeis.org/A147605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 26, i32 n <= 50, i64 n <= 98, i128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function:
   a(n) = -a(n-1) + a(n-3) + 2*a(n-4) + 3*a(n-5) + 5*a(n-6) + 7*a(n-7)
          + 5*a(n-8) + 3*a(n-9) + 2*a(n-10) + a(n-11) - a(n-13) - a(n-14)
   with a(0) = 1.  The coefficients come from the denominator
   P(x) = 1 + x - x^3 - 2x^4 - 3x^5 - 5x^6 - 7x^7 - 5x^8 - 3x^9 - 2x^10
           - x^11 + x^13 + x^14. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Coefficients p_k of the denominator P(x) for k=0..14.
       p_0 = 1, p_1 = 1, p_2 = 0, p_3 = -1, p_4 = -2, ... , p_14 = 1. */
    static const int coeff[15] = {
        1,  /* p0 */
        1,  /* p1 */
        0,  /* p2 */
        -1, /* p3 */
        -2, /* p4 */
        -3, /* p5 */
        -5, /* p6 */
        -7, /* p7 */
        -5, /* p8 */
        -3, /* p9 */
        -2, /* p10 */
        -1, /* p11 */
        0,  /* p12 */
        1,  /* p13 */
        1   /* p14 */
    };

    /* Cache to allow O(1) subsequent look‑ups for lower or previously
       computed indices.  The cache grows only when a larger n is
       requested. */
    static seq_t *cache = NULL;       /* dynamically allocated array */
    static idx_t  cache_len = 0;      /* number of elements currently stored */

    /* Initialize the cache with the first term a(0) = 1. */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 */
        cache[0] = 1;
    }

    /* If the requested index is beyond the current cache size, extend it. */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);          /* new array size */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                        /* allocation failure */
        cache = tmp;
        /* Compute terms from the current cache length up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            idx_t maxk = (i < 14 ? i : 14);        /* k runs 1..min(i,14) */
            for (idx_t k = 1; k <= maxk; ++k) {
                sum += (seq_t)coeff[k] * cache[i - k];
            }
            cache[i] = -sum;                       /* a(i) = -∑ coeff[k]*a(i-k) */
        }
        cache_len = new_len;
    }

    return cache[n];
}

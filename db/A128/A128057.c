/*
 * A128057  Expansion of (1+x)/sqrt(1+4x^2).
 * https://oeis.org/A128057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = coefficient of x^n in (1+x)/sqrt(1+4*x^2) Recurrence
 *            (D-finite, order 3): n*a(n) + (n-2)*a(n-1) + 4*(n-1)*a(n-2) +
 *            4*(n-3)*a(n-3) = 0 Initial values: a(0) = 1 a(1) = 1 a(2) = -2
 *            Computation uses memoisation to avoid recomputation for
 *            arbitrary call order
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 29, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function to be implemented */
seq_t a(idx_t n)
{
    /* static cache to store previously computed terms */
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;   /* number of elements currently filled (cache_len-1 is last index) */
    static size_t  cache_cap = 0;   /* allocated capacity */

    /* extend cache to include index n, if necessary */
    if (n >= cache_len) {
        /* increase capacity if needed */
        if (cache_cap < (size_t)(n + 1)) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)1;
            else if (i == 2) cache[i] = (seq_t)-2;
            else {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                /* numerator of the recurrence: -( (i-2)*a1 + 4*(i-1)*a2 + 4*(i-3)*a3 ) */
                seq_t num = -(((seq_t)(i-2))*a1 + (seq_t)4*((seq_t)(i-1))*a2 + (seq_t)4*((seq_t)(i-3))*a3);
                cache[i] = num / (seq_t)i;   /* division is exact */
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}

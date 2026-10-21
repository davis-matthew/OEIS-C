/*
 * A077942  Expansion of 1/(1-2*x+x^2+2*x^3).
 * https://oeis.org/A077942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - 2*a(n-3), with a(0)=1, a(1)=2, a(2)=3
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 42, i64 n <= 82, i128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the sequence Expansion of 1/(1‑2*x+x^2+2*x^3) (OEIS A077942) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values currently computed (>=0) */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    /* Resize cache if it cannot hold up to index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 2;
            } else if (i == 2) {
                cache[i] = 3;
            } else {
                cache[i] = ( 2 * cache[i-1]
                            -      cache[i-2]
                            - 2 * cache[i-3] );
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}

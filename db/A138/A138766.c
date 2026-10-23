/*
 * A138766  Real part of upper left and lower right terms of [1,(1+I); 1,1]^n
 *          * [1,0].
 * https://oeis.org/A138766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - a(n-4) Initial values: a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=7
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 29, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Return cached value if already computed */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Expand cache to hold terms up to n */
    idx_t new_len = n;
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    } else {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    }

    /* Compute new terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t idx = i - 1;          /* array index for term a(i)   */
        if (i == 1) {
            cache[idx] = (seq_t)1;
        } else if (i == 2) {
            cache[idx] = (seq_t)2;
        } else if (i == 3) {
            cache[idx] = (seq_t)4;
        } else if (i == 4) {
            cache[idx] = (seq_t)7;
        } else {
            /* a(i) = 4*a(i‑1) – 4*a(i‑2) – a(i‑4) */
            cache[idx] = (seq_t)4 * cache[idx - 1]
                       - (seq_t)4 * cache[idx - 2]
                       - cache[idx - 4];
        }
    }

    cache_len = n;
    return cache[n - 1];
}

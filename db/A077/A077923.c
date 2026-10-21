/*
 * A077923  Expansion of (1-x)^(-1)/(1+2*x-x^2+2*x^3).
 * https://oeis.org/A077923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 a(n-4) - 3 a(n-3) + 3 a(n-2) - a(n-1), a(0)=1, a(1)=-1,
 *            a(2)=4, a(3)=-10
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to share results between calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Initialise cache with first four terms */
    if (cache_size == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)-10;
        cache_size = 4;
    }

    /* Extend capacity if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = (seq_t)2 * cache[i-4]
                        - (seq_t)3 * cache[i-3]
                        + (seq_t)3 * cache[i-2]
                        - cache[i-1];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}

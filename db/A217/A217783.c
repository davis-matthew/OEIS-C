/*
 * A217783  Expansion of (1 - x)*(1 - 2*x)/((1 - 5*x + 5*x^2)*(1 - 3*x +
 *          x^2)).
 * https://oeis.org/A217783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=21, a(3)=83,
 *            a(n)=8*a(n-1)-21*a(n-2)+20*a(n-3)-5*a(n-4) for n>=4
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache to store already computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* allocated capacity (in seq_t units) */

    /* Ensure the cache can hold value for index n */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first four terms on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)83;
        cache_len = 4;
    }

    /* Compute terms up to n, if not already computed */
    while (cache_len <= n) {
        /* Use seq_t arithmetic to avoid overflow on big types */
        seq_t v = cache[cache_len - 1];
        v *= (seq_t)8;
        seq_t temp;

        temp = cache[cache_len - 2];
        temp *= (seq_t)21;
        v -= temp;

        temp = cache[cache_len - 3];
        temp *= (seq_t)20;
        v += temp;

        temp = cache[cache_len - 4];
        temp *= (seq_t)5;
        v -= temp;

        cache[cache_len] = v;
        ++cache_len;
    }

    return cache[n];
}

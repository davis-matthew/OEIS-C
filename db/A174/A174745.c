/*
 * A174745  y-values in the solution to x^2 - 21*y^2 = 1.
 * https://oeis.org/A174745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110*a(n-1)-a(n-2), a(1)=0, a(2)=12
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc, size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values, index 0 unused */
    static size_t cache_cap = 0;    /* current capacity of cache */
    static idx_t cache_len = 0;     /* highest index value computed so far */

    /* Initial allocation if needed */
    if (!cache) {
        cache_cap = 3;                         /* need indices 0,1,2 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;           /* unlikely, safe fallback */
    }

    /* Base values, set once */
    if (cache_len == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        cache_len = 2;
    }

    /* Grow cache if n is beyond current capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;           /* fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t v1 = cache[(size_t)(i - 1)];
        seq_t v2 = cache[(size_t)(i - 2)];
        cache[(size_t)i] = ((seq_t)110 * v1) - v2;
    }
    if (n > cache_len)
        cache_len = n;

    return cache[(size_t)n];
}

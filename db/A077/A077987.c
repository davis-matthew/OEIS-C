/*
 * A077987  Expansion of 1/(1+2*x-x^2+2*x^3).
 * https://oeis.org/A077987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1)+a(n-2)-2*a(n-3), a(0)=1, a(1)=-2, a(2)=5
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic cache for memoization */
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* initialise the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* out‑of‑memory guard */
        cache[0] = 1;
        cache[1] = -2;
        cache[2] = 5;
    }

    /* already computed? */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to hold terms up to n */
    idx_t new_len = n + 1;
    if (new_len < 3) new_len = 3;
    size_t new_size = (size_t)new_len * sizeof(seq_t);
    seq_t *tmp = (seq_t*)realloc(cache, new_size);
    if (!tmp) return 0;       /* out‑of‑memory guard */
    cache = tmp;

    /* compute all missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = - (cache[i - 1] * 2) + cache[i - 2] - (cache[i - 3] * 2);
    }
    cache_len = new_len;

    return cache[n];
}

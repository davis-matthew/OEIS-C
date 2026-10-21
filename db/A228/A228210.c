/*
 * A228210  x-values in the solutions to x^2 - 5y^2 = 44.
 * https://oeis.org/A228210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-4); a(1)=7, a(2)=8, a(3)=13, a(4)=17
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* ensure we have enough space for indices up to n */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while ((size_t)n > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)      cache[i-1] = (seq_t)7;
        else if (i == 2) cache[i-1] = (seq_t)8;
        else if (i == 3) cache[i-1] = (seq_t)13;
        else if (i == 4) cache[i-1] = (seq_t)17;
        else {
            seq_t a1 = cache[i-3]; /* a(i-2) */
            seq_t a2 = cache[i-5]; /* a(i-4) */
            cache[i-1] = (seq_t)(3 * a1 - a2);
        }
    }
    cache_len = (cache_len < n) ? n : cache_len;

    return cache[n-1];
}

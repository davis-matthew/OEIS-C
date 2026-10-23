/*
 * A162557  a(n) = ((3+sqrt(3))*(4+sqrt(3))^n+(3-sqrt(3))*(4-sqrt(3))^n)/6.
 * https://oeis.org/A162557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3+√3)(4+√3)^n + (3-√3)(4-√3)^n)/6 Recurrence: a(0)=1,
 *            a(1)=5, a(n)=8*a(n-1)-13*a(n-2) for n>1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;   /* number of pre‑computed entries */

    /* ensure the array can hold indices up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise the first terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {               /* n==0 already handled */
        cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* compute missing terms up to n (if any) */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)8 * cache[i - 1] - (seq_t)13 * cache[i - 2];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}

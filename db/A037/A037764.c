/*
 * A037764  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,0,1,2.
 * https://oeis.org/A037764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-4) - 10*a(n-5) Initial values: a(1)=3,
 *            a(2)=30, a(3)=301, a(4)=3012, a(5)=30123
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed = 0;   /* largest index already computed */

    /* ensure the cache is large enough (idx_t is 64‑bit) */
    if (cache_cap < n + 1) {
        idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise the first five terms (safe to do each time) */
    if (computed < 5) {
        cache[1] = 3;
        cache[2] = 30;
        cache[3] = 301;
        cache[4] = 3012;
        cache[5] = 30123;
        computed = 5;
    }

    /* compute missing terms up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 5) {
                /* already set */
                continue;
            }
            cache[i] = 10 * cache[i - 1] + cache[i - 4] - 10 * cache[i - 5];
        }
        computed = n;
    }

    return cache[n];
}

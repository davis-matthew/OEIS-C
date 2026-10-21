/*
 * A089977  Expansion of 1/((1-2*x)*(1+x+2*x^2)).
 * https://oeis.org/A089977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) (for n >= 3), a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 129,
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
    static idx_t last_computed = 0;

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* OOM guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        last_computed = 2;
    }

    if (n <= last_computed)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* OOM guard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t mult4 = cache[i - 3] * (seq_t)4;
        cache[i] = cache[i - 1] + mult4;
    }
    last_computed = n;

    return cache[n];
}

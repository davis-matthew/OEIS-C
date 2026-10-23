/*
 * A028029  Expansion of 1/((1-3x)(1-4x)(1-5x)(1-10x)).
 * https://oeis.org/A028029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=22, a(2)=317, a(3)=3830;
 *            a(n)=22*a(n-1)-167*a(n-2)+530*a(n-3)-600*a(n-4) for n≥4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* sequence values */
    static size_t cache_cap = 0;      /* current capacity of cache */
    static idx_t cache_sz = 0;        /* number of values stored (cache_sz = highest index + 1) */

    /* Ensure capacity to store up to index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;       /* double until large enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = cache_sz; i <= n; ++i) {
        if (i == 0)           cache[i] = (seq_t)1;
        else if (i == 1)      cache[i] = (seq_t)22;
        else if (i == 2)      cache[i] = (seq_t)317;
        else if (i == 3)      cache[i] = (seq_t)3830;
        else {
            /* left part is always ≥ right part for all n≥4 */
            seq_t left  = 22 * cache[i-1] + 530 * cache[i-3];
            seq_t right = 167 * cache[i-2] + 600 * cache[i-4];
            cache[i] = left - right;
        }
    }
    cache_sz = n + 1;      /* update number of stored values */

    return cache[n];
}

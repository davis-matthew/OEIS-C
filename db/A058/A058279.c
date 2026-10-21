/*
 * A058279  a(0)=a(1)=1, a(n)=a(n-2)+(n+1)*a(n-1).
 * https://oeis.org/A058279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + (n+1) * a(n-1), a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (never freed) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise for n = 0, 1 */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)1;          /* a(1) */
    }

    /* already memoised */
    if (n < cache_size)
        return cache[n];

    /* grow cache up to n */
    /* realloc to new size (n+1) */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) { /* OOM – fall back to recomputing */
        cache = NULL; /* no cache retained */
        cache_size = 0;
        /* recompute iteratively without cache */
        seq_t a0 = (seq_t)1;
        seq_t a1 = (seq_t)1;
        if (n == 0) return a0;
        if (n == 1) return a1;
        seq_t ai;
        for (idx_t i = 2; i <= n; ++i) {
            ai = a0 + ((seq_t)(i + 1) * a1);
            a0 = a1;
            a1 = ai;
        }
        return a1;
    }
    cache = new_cache;

    /* compute from existing size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i - 2] + ((seq_t)(i + 1) * cache[i - 1]);
    }
    cache_size = n + 1;

    return cache[n];
}

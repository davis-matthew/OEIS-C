/*
 * A204766  a(n) = 167*(n-1)-a(n-1) with n>1, a(1)=13.
 * https://oeis.org/A204766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 167*(n-1) - a(n-1), with a(1)=13
 * Range:     u8 n <= 3, u16 n <= 785, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len  = 0;   /* highest index computed (1‑based) */
    static idx_t cache_alloc = 0;   /* allocated capacity (1‑based) */

    /* initialise cache with a(1) */
    if (cache_len == 0) {
        cache_alloc = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_alloc);
        if (!cache) abort();
        cache[0] = 13;
        cache_len = 1;
    }

    /* grow array if necessary */
    if (n > cache_alloc) {
        idx_t new_alloc = cache_alloc ? cache_alloc : 1;
        while (new_alloc < n) new_alloc <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_alloc);
        if (!tmp) abort();
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* compute missing terms up to n */
    while (cache_len < n) {
        idx_t i = cache_len + 1;              /* 1‑based index to compute */
        seq_t term = (seq_t)167 * (i - 1);    /* 167*(n-1)   */
        seq_t val  = term - cache[cache_len - 1]; /* term - a(n-1) */
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n - 1];
}

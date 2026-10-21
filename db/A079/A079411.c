/*
 * A079411  a(1)=1, a(n) = n - a(a(ceiling(n/2))).
 * https://oeis.org/A079411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 353, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A079411: a(1) = 1, a(n) = n - a(a(ceil(n/2))) for n >= 2.
 * This implementation uses memoisation to pre‑compute the sequence
 * up to the maximum n requested across all calls.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 1..max_computed */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;    /* highest n already computed */
    static idx_t cache_cap   = 0;      /* allocated size */

    if (n < 1) return 0; /* undefined, but return 0 safely */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;              /* arbitrary small start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed */
        cache[1] = (seq_t)1;        /* a(1) = 1 */
        max_computed = 1;
    }

    /* grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (n + 1 > new_cap) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t mid = (i + 1) / 2;                 /* ceil(i/2) */
        seq_t mid_val = cache[mid];              /* a(ceil(i/2)) */
        idx_t idx = (idx_t)mid_val;              /* used as index */
        seq_t inner = cache[idx];                /* a(a(ceil(i/2))) */
        cache[i] = (seq_t)i - inner;
    }
    max_computed = n;

    return cache[n];
}

/*
 * A065961  a(n) = (3*n - 1)!*n/2.
 * https://oeis.org/A065961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n)! / 6 * Recurrence: a(1) = 1 * a(n) =
 *            3*n*(3*n-1)*(3*n-2)*a(n-1) (n > 1) * The implementation caches
 *            computed values so that any call order * is efficient. It works
 *            for any seq_t type supported by the * build (fixed‑size integers
 *            or arbitrary precision)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cached values: cache[0] unused, cache[1] = a(1) = 1 */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;    /* maximum computed index */
    static idx_t cache_cap   = 0;    /* current allocated capacity */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                         /* indices 0 and 1 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)0;                  /* unused */
        cache[1]  = (seq_t)1;                  /* a(1) */
        cache_len = 1;
    }

    /* Grow capacity if needed */
    if (n > cache_cap - 1) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;    /* double until big enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t three_i = (seq_t)3 * (seq_t)i;          /* 3*i */
        seq_t tmp     = three_i * (three_i - 1);      /* 3*i*(3*i-1) */
        seq_t factor  = tmp * (three_i - 2);          /* 3*i*(3*i-1)*(3*i-2) */
        cache[i]      = cache[i - 1] * factor;        /* recurrence */
    }
    cache_len = (cache_len > n) ? cache_len : n;

    return cache[n];
}

/*
 * A217779  Expansion of (1 - 4*x + 4*x^2)/((1 - 5*x + 5*x^2)*(1 - 3*x +
 *          x^2)).
 * https://oeis.org/A217779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) Initial
 *            values: a(0) = 1, a(1) = 4, a(2) = 15, a(3) = 56
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A217779 (diagonal of A217770). */
seq_t a(idx_t n) {
    /* Static cache to store previously computed terms */
    static seq_t *cache      = NULL;   /* array of computed values         */
    static idx_t  cache_cap  = 0;       /* allocated size of cache         */
    static idx_t  max_comp   = 0;       /* highest index computed (∈ [0..]) */

    /* Ensure the cache is initialized with the first four terms */
    if (cache == NULL) {
        cache_cap = 4 + 1;                            /* store a0..a3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();                  /* out‑of‑memory */
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 15;
        cache[3] = 56;
        max_comp = 3;                               /* indexes 0..3 computed */
    }

    /* Grow cache if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();             /* out‑of‑memory */
        cache    = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > max_comp) {
        /* The recurrence is valid for n >= 4 */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* Sum of positive coefficients */
            seq_t pos = 8 * cache[i - 1] + 20 * cache[i - 3];
            /* Sum of negative coefficients */
            seq_t neg = 21 * cache[i - 2] + 5  * cache[i - 4];
            cache[i] = pos - neg;
        }
        max_comp = n;
    }

    return cache[n];
}

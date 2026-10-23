/*
 * A177369  Expansion of g.f.: (1+4*x-4*x^2)/(1-3*x-4*x^2+4*x^3).
 * https://oeis.org/A177369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=7, a(3)=21; a(n)=3*a(n-1)+4*a(n-2)-4*a(n-3) for n>=4 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Minimal cache to avoid recomputation; works for any call order. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t size  = 0;   /* number of entries already computed */

    /* Handle n < 1 – offset is 1.  Behaviour below offset does not matter. */
    if (n == 0) return (seq_t)0;

    /* Ensure cache holds at least 3 base values. */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)21;
        size   = 3;
    }

    /* If needed, grow the cache. */
    if ((idx_t)size < n) {
        if ((idx_t)cap < n) {
            size_t new_cap = cap * 2;
            while ((idx_t)new_cap < n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;    /* allocation failure – unlikely */
            cache = tmp;
            cap   = new_cap;
        }
        /* Compute terms up to n-1 (0‑based). */
        for (idx_t i = size; i < n; ++i) {
            cache[i] = 3 * cache[i - 1] + 4 * cache[i - 2] - 4 * cache[i - 3];
        }
        size = n;
    }

    return cache[n - 1];
}

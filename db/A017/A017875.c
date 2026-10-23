/*
 * A017875  Expansion of
 *          1/(1-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17).
 * https://oeis.org/A017875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=8}^{17} a(n-k) for n>16 Initial values: a(0)=1,
 *            a(n)=0 for 1≤n≤7
 * Range:     u8 n <= 40, u16 n <= 68, u32 n <= 125, u64 n <= 237,
 *            u128 n <= 463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure cache capacity for indices 0..n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;   /* double until enough */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* out of memory → 0, acceptable as per spec */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;                      /* a(0) = 1 */
        } else if (i < 8) {
            val = 0;                      /* a(1..7) = 0 */
        } else {
            seq_t sum = 0;
            /* Sum a(i-8) .. a(i-17) */
            for (int k = 8; k <= 17; ++k) {
                if (i >= (idx_t)k)
                    sum += cache[i - k];
            }
            val = sum;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}

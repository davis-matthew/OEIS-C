/*
 * A052671  Expansion of e.g.f. x^3*(1-x)/(1-2*x).
 * https://oeis.org/A052671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n <= 2, a(3) = 6, a(4) = 24, a(n) = 2*n*a(n-1) for
 *            n >= 5 (equivalently a(n) = 2^(n-4)*n! for n > 3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise cache up to index 4 with known values */
    if (cache == NULL) {
        cache_cap = 8;
        cache_len = 5;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* out of memory (unlikely in tests) */

        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 6;
        cache[4] = 24;
    }

    /* expand cache if required */
    if (n >= cache_len) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;    /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 3) {
            cache[i] = 6;
        } else if (i == 4) {
            cache[i] = 24;
        } else { /* i >= 5 */
            cache[i] = cache[i-1] * ((seq_t)2 * (seq_t)i);
        }
    }
    cache_len = n + 1;

    return cache[n];
}

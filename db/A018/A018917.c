/*
 * A018917  Define the generalized Pisot sequence T(a(0),a(1)) by: a(n+2) is
 *          the greatest integer such that a(n+2)/a(n+1) < a(n+1)/a(n). This
 *          is T(3,5).
 * https://oeis.org/A018917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) for n ≥ 4 Initial terms: a(0)=3,
 *            a(1)=5, a(2)=8, a(3)=12 This implies a(4)=17, a(5)=24, a(6)=33,
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 72, u64 n <= 150,
 *            u128 n <= 308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the generalized Pisot sequence T(3,5) – OEIS A018917. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Return directly for the first four terms */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)12;

    /* Initialise cache for the first four values if not already */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) abort();      /* out‑of‑memory is fatal */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)12;
    }

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (new_cache == NULL) abort();   /* out‑of‑memory */
        cache = new_cache;
        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] - cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}

/*
 * A131882  a(0)=1; thereafter a(n)=a(n-1)+a([n/Phi]), where
 *          Phi=(1+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A131882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(n / Φ)), a(0) = 1, Φ = (1+√5)/2
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 297, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoized values */
static idx_t  cached_len = 0;        /* number of initialized entries */
static idx_t  cached_cap = 0;        /* allocated capacity */

/* Compute floor(n / Φ) using high‑accuracy long double arithmetic. */
static idx_t floor_div_phi(idx_t n) {
    /* Φ−1 = (√5−1)/2 ≈ 0.6180339887498948482045868 */
    static const long double PHI_INV = 0.6180339887498948482045868343656L;
    /* add a tiny epsilon to guard against rounding down when the real
       fractional part is very close to 1.0 */
    return (idx_t)((long double)n * PHI_INV + 1e-12L);
}

/* Ensure the cache is computed up to index n. */
static void ensure_cache(idx_t n) {
    if (cached_len > n) return;

    /* grow capacity if necessary */
    if (cached_cap < 16) cached_cap = 16;
    while (cached_cap <= n) cached_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
    }

    /* start computing from the first missing index */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            idx_t k = floor_div_phi(i);
            cache[i] = cache[i - 1] + cache[k];
        }
    }
    cached_len = n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}

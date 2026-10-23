/*
 * A132862  Number of permutations divided by the number of (binary) heaps on
 *          n elements.
 * https://oeis.org/A132862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(n) = n * a(f) * a(n-1-f) (n > 0) * where f =
 *            min(b-1, n - b/2) with b = 2^⌊log2(n)⌋
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 51, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Finds floor(log2(v)) for v > 0 */
static inline idx_t floor_log2(idx_t v) {
    idx_t r = 0;
    while (v > 1) {
        v >>= 1;
        r++;
    }
    return r;
}

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;     /* array of seq_t values */
    static idx_t cache_len = 0;     /* allocated length */
    static idx_t last_computed = 0; /* last index computed */

    /* initialize cache for n = 0 */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;   /* out of memory; return 0 */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last_computed = 0;
    }

    /* compute values up to requested n */
    if (n > last_computed) {
        /* enlarge cache if necessary */
        if (n + 1 > cache_len) {
            idx_t new_len = n + 1;
            seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
            if (!new_cache) return (seq_t)0;  /* realloc failed */
            cache = new_cache;
            cache_len = new_len;
        }

        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* compute f for this i */
            idx_t k = floor_log2(i);
            idx_t b = (idx_t)1U << k;       /* 2^k */
            idx_t half_b = b >> 1;          /* b/2 */
            idx_t f1 = b - 1;
            idx_t f2 = i - half_b;
            idx_t f = (f1 < f2) ? f1 : f2;

            /* recurrence */
            seq_t term1 = (seq_t)i;
            seq_t term2 = cache[f];
            seq_t term3 = cache[i - 1 - f];
            cache[i] = term1 * term2 * term3;
        }
        last_computed = n;
    }

    return cache[n];
}

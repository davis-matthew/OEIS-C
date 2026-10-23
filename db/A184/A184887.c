/*
 * A184887  a(n) = (8^n/n!^2) * Product_{k=0..n-1} (16k+3)*(16k+5).
 * https://oeis.org/A184887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8^n/n!^2) * ∏_{k=0}^{n-1}(16k+3)(16k+5)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* local gcd for seq_t types */
static inline seq_t seq_gcd_local(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* cache_len is the number of cached terms */

    /* Ensure the cache holds indices up to n */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache && new_len > 0) {
            /* allocation failure – abort */
            abort();
        }
        cache = new_cache;

        /* Initialize new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == (idx_t)0) {
                cache[0] = (seq_t)1;
                continue;
            }

            /* previous term */
            seq_t prev = cache[i - 1];

            /* Compute numerator = 8 * (16*i - 13) * (16*i - 11) */
            seq_t ii = (seq_t)i;
            seq_t nm1 = (seq_t)(16) * ii - (seq_t)13;  /* 16*i - 13 */
            seq_t nm2 = (seq_t)(16) * ii - (seq_t)11;  /* 16*i - 11 */
            seq_t numerator = (seq_t)8 * nm1;
            numerator *= nm2;

            /* denominator = i * i */
            seq_t denom = ii * ii;

            /* Compute result = prev * numerator / denom (exact division) */
            seq_t val = prev * numerator;
            val /= denom;

            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}

/*
 * A076005  Fifth column of triangle A075503.
 * https://oeis.org/A076005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^n) * S2(n+5, 5) (Stirling numbers of the second kind)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A076005 (Fifth column of triangle A075503).
 * The implementation computes Stirling numbers of the second kind S2(m,k)
 * incrementally for k=5, avoiding negative coefficients and using only
 * addition and multiplication, which works with all supported seq_t types.
 * Memoisation is used so each index is computed at most once and the
 * function handles calls in any order.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state */
    static seq_t *cache = NULL;           /* memoised values a(0..cached_len-1) */
    static size_t cached_len = 0;         /* number of terms stored */
    static seq_t pow8 = 0;                /* 8^n for the current n */
    static seq_t s[6] = {0};              /* s[k] = S2(current_m, k) */
    static int initialized = 0;           /* one‑time initialisation flag */

    if (!initialized) {
        /* initialise Stirling numbers up to m = 5 (k=5) */
        for (int k = 0; k <= 5; ++k) s[k] = 0;
        s[0] = 1; /* S2(0,0) = 1 */

        /* iterate m from 1 to 5 */
        for (idx_t m = 1; m <= 5; ++m) {
            for (int k = 5; k >= 1; --k) {
                s[k] = (seq_t)k * s[k] + s[k - 1];
            }
            s[0] = 0; /* S2(m,0) = 0 for m>0 */
        }

        pow8 = 1; /* 8^0 */
        /* allocate cache for a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0; /* allocation failure; caller not expected to handle */
        cache[0] = pow8 * s[5]; /* a(0) = 1 */
        cached_len = 1;
        initialized = 1;
    }

    /* If already computed, return immediately */
    if (n < cached_len) return cache[n];

    /* Ensure cache has enough space */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0; /* allocation failure */
    cache = new_cache;

    /* Extend cache up to n */
    for (idx_t idx = cached_len; idx <= n; ++idx) {
        /* Move to next m (current_m = idx + 5) */
        for (int k = 5; k >= 1; --k) {
            s[k] = (seq_t)k * s[k] + s[k - 1];
        }
        s[0] = 0;

        /* Update 8^idx */
        pow8 = pow8 * (seq_t)8;

        /* Store a(idx) */
        cache[idx] = pow8 * s[5];
    }

    cached_len = n + 1;
    return cache[n];
}

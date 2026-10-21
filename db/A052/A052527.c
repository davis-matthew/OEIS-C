/*
 * A052527  Expansion of (1-x)/(1-x-x^2-x^3+x^4).
 * https://oeis.org/A052527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4), a(0)=1, a(1)=0,
 *            a(2)=1, a(3)=2
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 83, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* allocated capacity */
    static size_t len = 0;    /* number of terms stored (>= 0) */

    /* helper to ensure capacity >= new_cap */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* memory allocation failed; return 0 as fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* If no terms stored yet, initialize the first four terms */
    if (len == 0) {
        /* Ensure indices 0..3 are available */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        len = 4;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence uses previous four terms, all already computed */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-4];
    }
    len = n > len ? (size_t)(n+1) : len;

    return cache[n];
}

/*
 * A121323  a(n) = (2*n+1)*a(n-1) - a(n-2) starting a(0)=0, a(1)=1.
 * https://oeis.org/A121323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*a(n-1) - a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 258 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache to memoise previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache for the base cases if empty */
    if (cache_len == 0) {
        cache = mem_new(seq_t, 2);
        if (!cache) return (seq_t)0;         /* out‑of‑memory fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Grow the cache if we need more space */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp) return (seq_t)0;           /* out‑of‑memory fallback */
        cache = tmp;
        /* Compute newly required terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t factor = (seq_t)(2 * i + 1);
            cache[i] = factor * cache[i - 1] - cache[i - 2];
        }
        cache_len = (idx_t)new_len;
    }

    return cache[n];
}

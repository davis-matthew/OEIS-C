/*
 * A140495  Union of A052103, A052102 and A052101, uniqued and sorted.
 * https://oeis.org/A140495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 78, u64 n <= 160,
 *            u128 n <= 323, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A140495: Union of A052103, A052102 and A052101 (uniqued and sorted).
 * Recurrence:
 *   a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 6, a(5) = 9, a(6) = 12,
 *   a(7) = 15, a(8) = 21, a(9) = 27, a(10) = 36, a(11) = 45, a(12) = 63,
 *   for n ≥ 13 :  a(n) = 3*a(n‑3) – 3*a(n‑6) + 3*a(n‑9).
 * The sequence is computed lazily and cached, so repeated calls are fast.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values a(0) … a(12) */
    static const seq_t init[13] = {
        0, 1, 2, 3, 6, 9, 12, 15, 21, 27, 36, 45, 63
    };

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms currently stored */

    /* If we need more terms, extend the cache and compute them */
    if (cache_len <= n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) { /* memory failure – return 0 as safe fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init[i];
            } else {
                seq_t t1 = 3 * cache[i - 3];
                seq_t t2 = 3 * cache[i - 6];
                seq_t t3 = 3 * cache[i - 9];
                cache[i] = t1 - t2 + t3;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}

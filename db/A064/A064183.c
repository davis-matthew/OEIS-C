/*
 * A064183  Define a pair of sequences by p(0) = 0, q(0) = p(1) = q(1) = 1,
 *          q(n+1) = p(n)*q(n-1), p(n+1) = q(n+1) + q(n) for n > 0; then a(n)
 *          = q(n) and A064526(n) = p(n).
 * https://oeis.org/A064183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n‑1) + a(n‑2)) * a(n‑2) a(0) = 1, a(1) = 1, a(2) = 1
 *            (OEIS A064183)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;      /* pointer to the array of terms */
    static idx_t cache_len = 0;      /* number of entries allocated */

    /* Fast return for the first three terms */
    if (n < 3)
        return (seq_t)1;

    /* Ensure the cache is large enough */
    if ((idx_t)(n + 1) > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        if (new_len < 64)            /* avoid too small allocations */
            new_len = 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        /* Initialise new slots to zero (uncomputed) */
        for (idx_t i = old_len; i < new_len; ++i)
            cache[i] = 0;
        cache_len = new_len;
    }

    /* Initialise base values if not already set */
    if (cache[0] == 0) cache[0] = (seq_t)1;
    if (cache[1] == 0) cache[1] = (seq_t)1;
    if (cache[2] == 0) cache[2] = (seq_t)1;

    /* Compute any missing terms up to n */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] == 0) {
            /* recurrence: a(n) = (a(n-1) + a(n-2)) * a(n-2) */
            cache[i] = (cache[i-1] + cache[i-2]) * cache[i-2];
        }
    }

    return cache[n];
}

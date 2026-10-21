/*
 * A051188  Sept-factorial numbers.
 * https://oeis.org/A051188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static idx_t cache_size = 0;
    static seq_t *cache = NULL;

    /* Grow the cache if needed */
    if (n >= cache_size) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* out of memory: abort */
            exit(1);
        }
        cache = new_cache;

        /* Initialize first element if this is the first allocation */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;   /* a(0) = 1 */
        }

        /* Compute missing terms using the recurrence a(k) = a(k‑1) * 7*k */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) continue;   /* already set */
            seq_t mult = ((seq_t)7) * (seq_t)i;
            cache[i] = cache[i - 1] * mult;
        }
        cache_size = n + 1;
    }
    return cache[n];
}

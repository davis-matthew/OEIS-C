/*
 * A097314  Pell equation solutions (3*a(n))^2 - 10*b(n)^2 = -1 with b(n) =
 *          A097315(n), n >= 0.
 * https://oeis.org/A097314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 39 a(n) = 38*a(n‑1) – a(n‑2) for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;  /* number of terms stored */
    static idx_t  cache_cap  = 0;  /* allocated capacity */

    /* initialise cache with first two terms */
    if (cache_cap == 0) {
        cache_cap = 16;                    /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)39;
        cache_len = 2;
    }

    /* extend cache if necessary */
    if (n >= cache_len) {
        if (n >= cache_cap) {
            /* grow cache – double until large enough */
            idx_t new_cap = cache_cap;
            while (new_cap <= n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prod = (seq_t)38 * cache[i-1];
            cache[i] = prod - cache[i-2];
        }
        cache_len = n + 1;
    }
    return cache[n];
}

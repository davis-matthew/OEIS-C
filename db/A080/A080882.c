/*
 * A080882  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0)=1, a(1)=3, a(2)=7.
 * https://oeis.org/A080882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) - 4*a(n-4) for n >= 4 Base values: a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=22
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static const seq_t base_values[4] = {
    (seq_t)1,
    (seq_t)3,
    (seq_t)7,
    (seq_t)22
};

seq_t a(idx_t n) {
    /* Initialize cache with the first four terms if needed */
    if (!cache) {
        /* Need space for indices 0..3 */
        ensure_cache((idx_t)3, &cache, &cache_cap);
        for (size_t i = 0; i < 4; ++i) cache[i] = base_values[i];
        cache_len = 4;  /* we have 0..3 stored */
    }

    /* For n < 4, return the precalculated base value */
    if ((idx_t)cache_len > n) {
        return cache[(size_t)n];
    }

    /* Compute terms up to n using recurrence */
    if (cache_len <= (size_t)n) {
        ensure_cache(n, &cache, &cache_cap);
        for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
            seq_t val = cache[idx - 2] * (seq_t)8;
            val -= cache[idx - 4] * (seq_t)4;
            cache[idx] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}

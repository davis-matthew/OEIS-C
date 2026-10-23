/*
 * A186751  a(0) = 3; thereafter, a(n) = a(n-1)^2 - 4.
 * https://oeis.org/A186751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3; for n >= 1, a(n) = a(n-1)^2 - 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_CACHE_N 10000      /* Maximum index for which we cache values */

static seq_t  cache[MAX_CACHE_N + 1];
static idx_t  last_cached = 0u;   /* Index of the last computed cache entry */
static bool   cache_init  = false;

/* Ensure the cache is initialized with a(0) */
static void init_cache(void) {
    if (!cache_init) {
        cache[0] = (seq_t)3;  /* a(0) = 3 */
        last_cached = 0;
        cache_init = true;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    if (n <= MAX_CACHE_N) {
        /* Compute up to n using the cache */
        if (n > last_cached) {
            for (idx_t i = last_cached + 1; i <= n; ++i) {
                cache[i] = cache[i - 1] * cache[i - 1] - (seq_t)4;
            }
            last_cached = n;
        }
        return cache[n];
    } else {
        /* n larger than cache size: compute iteratively without caching */
        seq_t val = (seq_t)3;
        for (idx_t i = 1; i <= n; ++i) {
            val = val * val - (seq_t)4;
        }
        return val;
    }
}

/*
 * A017866  Expansion of
 *          1/(1-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17).
 * https://oeis.org/A017866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)+a(n-16)+a(n-17)
 *            for n > 16. Initial values: a(0) = 1 a(1) = a(2) = a(3) = a(4) =
 *            a(5) = a(6) = 0 a(7) = a(8) = a(9) = a(10) = a(11) = a(12) =
 *            a(13) = 1 a(14) = 2, a(15) = 3, a(16) = 4, a(17) = 5 (The
 *            recurrence holds for...
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 111, u64 n <= 213,
 *            u128 n <= 415, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"

/* cache for already-computed values */
static seq_t *cache = NULL;          /* pointer to cached sequence */
static size_t cache_cap = 0;         /* capacity of cache (number of elements) */
static idx_t cached_n = 0;           /* largest index that has been computed */
static bool init_done = false;       /* lazy initialization flag */

/* initial values a(0) … a(17) */
static const seq_t init_vals[18] = {
    1ULL,          /* a(0) */
    0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, /* a(1)…a(6) */
    1ULL, 1ULL, 1ULL, 1ULL, 1ULL, 1ULL, 1ULL, /* a(7)…a(13) */
    2ULL,          /* a(14) */
    3ULL,          /* a(15) */
    4ULL,          /* a(16) */
    5ULL           /* a(17) */
};

seq_t a(idx_t n)
{
    /* Lazy initialization of cache with first 18 terms */
    if (!init_done) {
        ensure_cache(18, &cache, &cache_cap);
        for (idx_t i = 0; i < 18; ++i)
            cache[i] = init_vals[i];
        cached_n = 17;
        init_done = true;
    }

    /* If requested index already computed, return it */
    if (n <= cached_n)
        return cache[n];

    /* Grow cache if necessary */
    ensure_cache((size_t)n + 1, &cache, &cache_cap);

    /* Compute missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 7; k <= 17; ++k) {
            if (i >= k)
                sum += cache[i - k];
        }
        cache[i] = sum;
    }
    cached_n = n;
    return cache[n];
}

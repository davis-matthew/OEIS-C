/*
 * A144135  a(n) = GegenbauerC[n,2,8].
 * https://oeis.org/A144135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144135
 * a(n) = GegenbauerC[n,2,8]
 * Recurrence (valid for n >= 4):
 *   a(n) = 32*a(n-1) - 258*a(n-2) + 32*a(n-3) - a(n-4)
 * Initial values:
 *   a(0) = 1,   a(1) = 32,   a(2) = 766,   a(3) = 16288
 */

#include <stdlib.h>   /* size_t, malloc, realloc */

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of computed terms (>= 4 once initialised) */
static size_t cache_cap = 0;   /* allocated capacity */

/* enlarge the cache to hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the cache with the four seed values */
static void init_cache(void)
{
    if (cache_len > 0) return;          /* already initialised */
    ensure_capacity(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)32;
    cache[2] = (seq_t)766;
    cache[3] = (seq_t)16288;
    cache_len = 4;
}

seq_t a(idx_t n)
{
    /* Ensure seeds are present */
    init_cache();

    /* Return a cached value if available */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* Compute missing values up to n */
    size_t target = (size_t)n;
    ensure_capacity(target + 1);
    for (size_t i = cache_len; i <= target; ++i) {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = cache[i-4];
        seq_t val = 32 * v1 - 258 * v2 + 32 * v3 - v4;
        cache[i] = val;
    }
    cache_len = target + 1;
    return cache[target];
}

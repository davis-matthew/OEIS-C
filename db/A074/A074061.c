/*
 * A074061  Positive integers k such that 24*k^2 - 23 is a square.
 * https://oeis.org/A074061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) - a(n-4) (n ≥ 4) Initial values: a(0)=1,
 *            a(1)=4, a(2)=6, a(3)=39 This recurrence comes from the Pell‑type
 *            equation 24*k²‑23 = m². The function memoizes results in a
 *            growable static array so that any call order (not necessarily
 *            increasing) is handled efficiently
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms */
static size_t cache_cap = 0;  /* current capacity of cache */
static size_t cache_len = 0;  /* number of terms already initialized */

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache(size_t need)
{
    if (need < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first four terms (only when the function is called the first time). */
static void init_cache(void)
{
    ensure_cache(3);  /* need indices 0..3 */
    cache[0] = 1;
    cache[1] = 4;
    cache[2] = 6;
    cache[3] = 39;
    cache_len = 4;
}

seq_t a(idx_t n)
{
    /* Convert idx_t to size_t for array indexing; idx_t is unsigned, so this is safe. */
    size_t idx = (size_t)n;

    /* First call: set up the cache. */
    if (cache_len == 0) init_cache();

    /* Expand cache if necessary. */
    if (idx >= cache_cap) ensure_cache(idx);

    /* Compute terms up to idx if they haven't been computed yet. */
    for (size_t i = cache_len; i <= idx; ++i)
    {
        if (i < 4) continue;                  /* already initialised */
        seq_t t = cache[i - 2];
        t *= 10;                               /* multiply by the constant 10 */
        t -= cache[i - 4];                     /* subtract a(i-4) */
        cache[i] = t;
    }
    cache_len = idx + 1;  /* we now have terms up to idx */

    return cache[idx];
}

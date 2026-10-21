/*
 * A209807  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          two distinct values, and new values 0..3 introduced in row major
 *          order.
 * https://oeis.org/A209807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7, a(2)=34, a(3)=173, a(4)=930 a(n) = 8*a(n-1) - 7*a(n-2) -
 *            28*a(n-3) - 12*a(n-4) (n>=5)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* index 0 unused, cache[1] = a(1) */
static size_t cache_cap = 0;    /* allocated size minus one (index 0 unused) */
static size_t cache_len = 0;    /* highest index currently filled */

/* Ensure the cache can hold indices up to n (1‑based). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n <= cache_len) return;

    size_t needed = (size_t)n + 1;          /* include index 0 */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap) * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initial terms, if not already done. */
    if (cache_len < 4) {
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)34;
        cache[3] = (seq_t)173;
        cache[4] = (seq_t)930;
        cache_len = 4;
    }

    /* Compute new terms using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t t = 8 * cache[i-1] - 7 * cache[i-2] - 28 * cache[i-3] - 12 * cache[i-4];
        cache[i] = t;
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1, return 0 as safe default */
    ensure_cache_size(n);
    return cache[n];
}

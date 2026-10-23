/*
 * A014010  Linear recursion relative of Shallit sequence S(2,6).
 * https://oeis.org/A014010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) + a(n-4) - 3*a(n-5) Initial
 *            values: a(0)=2, a(1)=6, a(2)=19, a(3)=61, a(4)=196
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* table of already computed terms */
static size_t cache_cap = 0;  /* how many slots are currently allocated */
static idx_t cache_size = 0;  /* number of terms stored (valid indices are 0 .. cache_size-1) */

/* Ensure the cache can hold all indices up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    if (n < (idx_t)cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    /* realloc works in both C and C++ when casted */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Lazily initialise the cache with the first five terms */
    if (cache == NULL) {
        ensure_cache_capacity(4);          /* need indices 0..4 -> at least 5 slots */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)19;
        cache[3] = (seq_t)61;
        cache[4] = (seq_t)196;
        cache_size = 5;
    }

    if (n < cache_size)
        return cache[n];

    /* Compute all missing terms up to n */
    ensure_cache_capacity(n);
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 3 * cache[i - 1];
        val +=     cache[i - 2];
        val -=     cache[i - 3];
        val +=     cache[i - 4];
        val -= 3 * cache[i - 5];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}

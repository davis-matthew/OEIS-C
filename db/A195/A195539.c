/*
 * A195539  Numerators b(n) of Pythagorean approximations b(n)/a(n) to
 *          sqrt(8).
 * https://oeis.org/A195539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34*a(n-2) - a(n-4) with initial terms: a(1)=12, a(2)=35,
 *            a(3)=408, a(4)=1189
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached values: cache[i] holds a(i) for i >= 1, cache[0] unused */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* highest n for which cache[n] is valid */
static idx_t cache_cap = 0;   /* current capacity of cache array */

/* ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap));
        if (!new_cache) abort();   /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* main sequence accessor */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined offset handling */

    /* initialize base terms if not yet set */
    if (cache_len < 4) {
        /* allocate minimal capacity */
        ensure_cache_capacity(4);
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)35;
        cache[3] = (seq_t)408;
        cache[4] = (seq_t)1189;
        cache_len = 4;
    }

    if (n <= cache_len) return cache[n];

    /* compute up to requested n */
    ensure_cache_capacity(n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 4) continue;  /* safety, values already set */
        seq_t v = (seq_t)34 * cache[i - 2] - cache[i - 4];
        cache[i] = v;
    }
    cache_len = n;
    return cache[n];
}

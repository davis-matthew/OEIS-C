/*
 * A078021  Expansion of (1-x)/(1-x+2*x^2+x^3).
 * https://oeis.org/A078021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - 2*a(n-2) - a(n-3); a(0)=1, a(1)=0, a(2)=-2
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 46, i64 n <= 94, i128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* allocated length of cache */
static idx_t max_cached = (idx_t)(-1); /* highest index in cache, (idx_t)-1 means none */

/* ensure cache is large enough for indices 0..n inclusive */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_len >= needed)
        return;

    size_t new_len = cache_len ? cache_len << 1 : 1;
    while (new_len < needed)
        new_len <<= 1;

    seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!p) abort();          /* out of memory */
    cache = p;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* 1. base terms handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-2;

    /* 2. initialize cache if first call */
    if (max_cached == (idx_t)(-1)) {
        ensure_cache_size(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-2;
        max_cached = 2;
    }

    /* 3. if n already cached, return it */
    if ((size_t)n <= max_cached)
        return cache[(size_t)n];

    /* 4. compute missing terms up to n */
    ensure_cache_size(n);
    for (size_t i = max_cached + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] - (seq_t)((seq_t)2 * cache[i-2]) - cache[i-3];
    }
    max_cached = n;

    return cache[(size_t)n];
}

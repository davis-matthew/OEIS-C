/*
 * A163063  Lucas(3n+2) = Fibonacci(3n+1) + Fibonacci(3n+3).
 * https://oeis.org/A163063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+a(n-2), a(0) = 3, a(1) = 11;
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity of the allocated array */
static size_t cache_len = 0;   /* number of values that have been computed and stored */

/* Make sure the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache(size_t need)
{
    if (need < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function computing the sequence. */
seq_t a(idx_t n)
{
    /* Initialise base cases only once. */
    if (cache_len == 0) {
        ensure_cache(1);           /* needs at least indices 0 and 1 */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)11;
        cache_len = 2;
    } else if (cache_len == 1) {
        cache[1] = (seq_t)11;
        cache_len = 2;
    } else {
        /* If the cache already contains the base values, nothing to do. */
    }

    /* Return already computed value if available. */
    if (n < cache_len) return cache[n];

    /* Compute the missing terms up to n. */
    ensure_cache((size_t)n);
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] + cache[i-2];
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}

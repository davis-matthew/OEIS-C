/*
 * A081192  10th binomial transform of (1,0,1,0,1,......), A059841.
 * https://oeis.org/A081192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - 99*a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void grow_cache_to(size_t n)
{
    if (n < cache_len)
        return;

    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1)
            new_cap = n + 1;
        /* Cast is required to compile as C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Newly allocated entries are initialized lazily later */
}

seq_t a(idx_t n)
{
    /* Ensure the cache can hold index n */
    grow_cache_to((size_t)n);

    /* Initialize base values lazily */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)10;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)20 * cache[i - 1] - (seq_t)99 * cache[i - 2];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}

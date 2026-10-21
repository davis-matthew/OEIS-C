/*
 * A056123  a(n) = 3*a(n-1) - a(n-2) with a(0)=1, a(1)=11.
 * https://oeis.org/A056123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) with a(0) = 1, a(1) = 11
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of initialized elements in cache */
static idx_t cache_cap = 0;   /* capacity of allocated cache */

seq_t a(idx_t n) {
    /* Handle the two base cases immediately */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache_len = 2;
    }

    /* Grow the cache if the requested index is beyond current capacity */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* fallback on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n singularly (and store them) */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1] - cache[i-2];
    }
    cache_len = n + 1;

    return cache[n];
}

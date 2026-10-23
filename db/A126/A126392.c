/*
 * A126392  Number of base 5 n-digit numbers with adjacent digits differing by
 *          two or less.
 * https://oeis.org/A126392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-3) a(0) = 1, a(1) = 5, a(2) = 19
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise the cache with the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory; caller will see 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)19;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* extend cache to contain a[n] */
    size_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* out‑of‑memory */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= n; ++i) {
        /* 4 * a(i-1) - a(i-3) */
        seq_t val = (seq_t)4 * cache[i - 1];
        val = val - cache[i - 3];
        cache[i] = val;
    }
    cache_len = new_len;

    return cache[n];
}

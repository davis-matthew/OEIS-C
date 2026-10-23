/*
 * A163206  a(n) = 20*a(n-1) - 92*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A163206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 92*a(n-2); a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialize cache with the first two terms. */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                  /* a(0) */
        cache[1] = (seq_t)10;                 /* a(1) */
        cache_len = 2;
        if (n < 2) return cache[n];
    }

    /* Grow cache if necessary. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap < 2 ? 2 : cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)20 * cache[i-1] - (seq_t)92 * cache[i-2];
    }
    cache_len = n + 1;
    return cache[n];
}

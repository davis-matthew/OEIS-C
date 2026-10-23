/*
 * A042104  Numerators of continued fraction convergents to sqrt(577).
 * https://oeis.org/A042104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 48*a(n-1)+a(n-2); a(0)=24, a(1)=1153
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS sequence A042104
 * (Numerators of continued fraction convergents to sqrt(577)).
 * The function uses memoisation to avoid recomputing terms
 * for previously requested indices.
 */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries currently stored */

    /* Ensure the cache has at least two initial terms */
    if (cache_len == 0) {
        /* Allocate space for two elements */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)24;
        cache[1] = (seq_t)1153;
        cache_len = 2;
    }

    /* Grow cache and compute missing terms up to n */
    if (n >= cache_len) {
        /* Resize the cache to hold indices 0 .. n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely in tests */
        cache = tmp;

        /* Compute terms in order from cache_len to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)48 + cache[i - 2];
        }
        cache_len = new_cap;
    }

    return cache[n];
}

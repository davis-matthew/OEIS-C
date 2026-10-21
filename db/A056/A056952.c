/*
 * A056952  Numerators of continued fraction for alternating factorial.
 * https://oeis.org/A056952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = a(n-1) + (n/2)*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A056952: numerators of continued fraction for alternating factorial. */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache  = NULL; /* array [0..len-1] */
    static size_t  cache_len = 0;

    /* handle the trivial cases first */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* if we need more entries, grow the cache */
    if (cache_len <= n) {
        size_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) exit(1);               /* allocation failed */

        /* initialise the first two terms if we are starting */
        if (cache_len == 0) {
            cache[0] = 0;
            if (new_len > 1) cache[1] = 1;
            cache_len = 2;
        }

        /* compute any missing terms from cache_len up to n */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t prev          = cache[i - 1];
            seq_t prev_prev     = cache[i - 2];
            seq_t factor        = (seq_t)(i >> 1);   /* n/2 using integer division */
            cache[i] = prev + factor * prev_prev;
        }
        cache_len = new_len;
    }

    return cache[n];
}

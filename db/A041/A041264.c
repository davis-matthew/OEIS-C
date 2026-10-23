/*
 * A041264  Numerators of continued fraction convergents to sqrt(145).
 * https://oeis.org/A041264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 24*a(n-1) + a(n-2), a(0) = 12, a(1) = 289
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure we have space for indices 0..n */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;      /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base terms if they are not yet present */
    if (cache_len == 0) {
        cache[0] = (seq_t)12;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)289;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)24 * cache[i-1]) + cache[i-2];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}

/*
 * A061522  a(0) = 0; a(n) is obtained by incrementing each digit of a(n-1) by
 *          7.
 * https://oeis.org/A061522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = increment_digits(a(n-1)), a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   increment_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/increment_digits.h"

/* Cache used to handle out‑of‑order calls. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Initialise the cache with a(0). */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = increment_digits(cache[cache_len - 1]);
        cache_len++;
    }

    return cache[(size_t)n];
}

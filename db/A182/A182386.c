/*
 * A182386  a(0) = 1, a(n) = 1 - n * a(n-1).
 * https://oeis.org/A182386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(n) = 1 - n * a(n-1) for n >= 1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding already computed terms */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    /* enlarge cache if needed */
    if ((size_t)n >= cached_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* initialisation of the cache */
        if (cached_len == 0) {
            cache[0] = 1;               /* a(0) */
            cached_len = 1;
        }
        /* compute missing terms recursively */
        for (size_t i = cached_len; i < new_len; ++i) {
            cache[i] = 1 - ((seq_t)i * cache[i - 1]);
        }
        cached_len = new_len;
    }
    return cache[(size_t)n];
}

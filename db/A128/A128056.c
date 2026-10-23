/*
 * A128056  Hankel transform of A128057.
 * https://oeis.org/A128056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -16*a(n-4) - 12*a(n-2) with a(0)=1, a(1)=-3, a(2)=-6,
 *            a(3)=28
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 18, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow any call order and reuse computed values. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    /* Initialise the cache with the first four terms. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;           /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)(-3);
        cache[2] = (seq_t)(-6);
        cache[3] = (seq_t)28;
        cached_len = 4;
    }

    /* Return the value if it is already in the cache. */
    if (n < cached_len)
        return cache[n];

    /* Extend the cache to store values up to n. */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (new_cache == NULL)
        return (seq_t)0;                            /* allocation failed */

    cache = new_cache;

    /* Compute the new entries using the recurrence. */
    for (idx_t i = cached_len; i < new_len; ++i) {
        seq_t val = (-16) * cache[i - 4];
        val -= 12 * cache[i - 2];
        cache[i] = val;
    }

    cached_len = new_len;
    return cache[n];
}

/*
 * A059224  a(n) = 2^(n-3)*(n + 3)*(2*n - 3).
 * https://oeis.org/A059224
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3)
   with a(3) = 18, a(4) = 70, a(5) = 224. */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Base cases for indices below the offset are not used. */
    if (n < 3)
        return (seq_t)0;

    /* Ensure the cache can hold the requested index. */
    if (n >= cache_size) {
        size_t new_size = n + 1;
        /* Grow in chunks to reduce realloc calls. */
        if (cache != NULL) {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        }
        /* Initialize new slots. */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 3)
                cache[i] = (seq_t)18;
            else if (i == 4)
                cache[i] = (seq_t)70;
            else if (i == 5)
                cache[i] = (seq_t)224;
            else {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                /* Compute 6*a1 + 8*a3 - 12*a2 */
                seq_t term = (seq_t)6 * a1;          /* 6*a(n-1) */
                term += (seq_t)8 * a3;              /* + 8*a(n-3) */
                term -= (seq_t)12 * a2;             /* - 12*a(n-2) */
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}

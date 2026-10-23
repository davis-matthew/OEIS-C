/*
 * A023390  a(n+1) = a(n) written in base 7 (read in base 10); a(1) = 7.
 * https://oeis.org/A023390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 19, u64 n <= 24, u128 n <= 28,
 *            big any n
 * Helpers:   base7_to_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 7;
   a(n+1) = the decimal number whose digits are the base‑7 digits of a(n). */

#include <stdlib.h>
#include "../utils/base7_to_decimal.h"

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  cache[0] corresponds to a(1). */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;        /* number of terms stored */

    if (n == 0)            /* undefined for n < 1, but guard against it */
        return (seq_t)0;

    /* Grow the cache if necessary */
    if ((size_t)n > cached_len) {
        size_t new_len = (size_t)n;

        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failure */
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failure */
        }

        /* Initialise a(1) if it has not been set yet */
        if (cached_len == 0) {
            cache[0] = (seq_t)7;          /* a(1) = 7 */
            cached_len = 1;
        }

        /* Compute missing terms */
        for (size_t i = cached_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = base7_to_decimal(prev);
        }
        cached_len = new_len;
    }
    return cache[n - 1];
}

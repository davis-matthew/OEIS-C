/*
 * A084241  a(n) = -5*a(n-1)-4*a(n-2) with n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A084241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -5*a(n-1) - 4*a(n-2) (a(0)=0, a(1)=1)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamically growable cache */
static idx_t cache_len = 0;   /* number of entries currently stored */

seq_t a(idx_t n)
{
    /* If we have not yet computed up to index n, extend the cache */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* Initialise base cases if they have not been set yet */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;           /* a(0) */
        }
        if (new_len > 1 && cache_len < 2) {
            cache[1] = (seq_t)1;           /* a(1) */
        }

        /* Compute new terms using the recurrence */
        idx_t i = (cache_len < 2 ? 2 : cache_len);
        for (; i < new_len; ++i) {
            cache[i] = (seq_t)(-5) * cache[i - 1] - (seq_t)4 * cache[i - 2];
        }
        cache_len = new_len;
    }
    return cache[n];
}

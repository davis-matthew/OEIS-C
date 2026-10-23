/*
 * A132780  a(0)=1. a(n+1)=2*a(n)-A130151(n).
 * https://oeis.org/A132780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n+1) = 2*a(n) - A130151(n), where
   A130151(n) = (-1)^(floor(n/3))   ( = +1 when floor(n/3) even, -1 otherwise ) */
#include <stdlib.h>   /* for realloc */

seq_t a(idx_t n)
{
    /* static cache keeps previously computed values for arbitrary access order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    size_t new_len;

    if (cache_len <= n) {
        /* grow cache to hold indices up to n */
        new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;

        /* initialise the first term if necessary */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev   = cache[i - 1];
            seq_t next   = prev * (seq_t)2;       /* 2*a(i-1)   */
            idx_t  block = (i - 1) / 3;           /* floor((i-1)/3) */

            if ((block & (idx_t)1) == 0) {        /* even block → A130151 = +1 */
                next -= (seq_t)1;
            } else {                             /* odd block  → A130151 = -1 */
                next += (seq_t)1;
            }

            cache[i] = next;
        }
        cache_len = new_len;
    }

    return cache[n];
}

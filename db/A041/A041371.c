/*
 * A041371  Denominators of continued fraction convergents to sqrt(200).
 * https://oeis.org/A041371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 198*a(n-2) - a(n-4); initial values: a(0)=1, a(1)=7,
 *            a(2)=197, a(3)=1386
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cache_len = 0;  /* number of entries in `cache` */

    /* grow the cache to support index `n` */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return zero as sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        size_t old_len = cache_len;
        cache_len = new_len;

        /* initialise the first four terms (if not already done) */
        if (cache_len > 0) cache[0] = (seq_t)1;
        if (cache_len > 1) cache[1] = (seq_t)7;
        if (cache_len > 2) cache[2] = (seq_t)197;
        if (cache_len > 3) cache[3] = (seq_t)1386;

        /* compute missing terms using the recurrence */
        for (size_t i = old_len; i <= (size_t)n; ++i) {
            if (i < 4) continue;           /* base cases already set */
            cache[i] = (seq_t)198 * cache[i-2] - cache[i-4];
        }
    }

    return cache[(size_t)n];
}

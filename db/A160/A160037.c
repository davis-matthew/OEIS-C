/*
 * A160037  Numerator of Hermite(n, 11/25).
 * https://oeis.org/A160037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 22*a(n-1) - 1250*(n-1)*a(n-2) ; a(0)=1, a(1)=22
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization array */
static idx_t cache_len = 0;   /* number of elements stored */

seq_t a(idx_t n)
{
    /* Return cached value if already available. */
    if (n < cache_len) return cache[n];

    /* Grow the cache to hold values up to n. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failure is fatal */
    cache = tmp;

    /* Compute all new entries in increasing order. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)22;
        } else {
            seq_t t1 = (seq_t)22 * cache[i - 1];
            seq_t t2 = cache[i - 2] * (seq_t)1250 * (seq_t)(i - 1);
            cache[i] = t1 - t2;
        }
    }

    cache_len = new_len;
    return cache[n];
}

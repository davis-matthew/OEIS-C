/*
 * A160221  Numerator of Hermite(n, 23/28).
 * https://oeis.org/A160221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 23, a(n) = 23*a(n-1) - 392*(n-1)*a(n-2) for n
 *            >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache of previously computed values (static to retain between calls) */
    static seq_t *cache = NULL;
    static size_t cap = 0;                /* number of entries stored */

    /* grow the cache to at least n+1 entries */
    if (n + 1 > cap) {
        /* determine new size: double or just enough to fit n */
        size_t newcap = (cap == 0) ? 2 : cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure → abort */
        /* compute new entries */
        for (size_t i = cap; i < newcap; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)23;
            } else {
                /* a(i) = 23*a(i-1) - 392*(i-1)*a(i-2) */
                seq_t term1 = (seq_t)23 * cache[i - 1];
                seq_t term2 = (seq_t)392 * (seq_t)(i - 1) * cache[i - 2];
                cache[i] = term1 - term2;
            }
        }
        cap = newcap;
    }
    return cache[n];
}

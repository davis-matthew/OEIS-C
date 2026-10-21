/*
 * A090990  Number of meaningful differential operations of the n-th order on
 *          the space R^5.
 * https://oeis.org/A090990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+3) = a(n+2) + 2*a(n+1) - a(n), a(1)=5, a(2)=9, a(3)=16
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 73, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset */
    if (n < 1) return 0;

    static seq_t *cache = NULL;   /* store a(1) .. a(cap) */
    static size_t cap = 0;        /* number of terms cached */

    /* initialise the first three terms */
    if (cap == 0) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 5;          /* a(1) */
        cache[1] = 9;          /* a(2) */
        cache[2] = 16;         /* a(3) */
        cap = 3;
    }

    /* if the requested term is already cached */
    if ((size_t)n <= cap)
        return cache[n - 1];

    /* grow the cache to hold up to n */
    seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
    if (!tmp) return 0;
    cache = tmp;

    /* compute new terms using the recurrence */
    for (size_t i = cap; i < (size_t)n; ++i) {
        cache[i] = cache[i - 1] + 2 * cache[i - 2] - cache[i - 3];
    }

    cap = (size_t)n;
    return cache[n - 1];
}

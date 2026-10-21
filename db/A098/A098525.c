/*
 * A098525  Expansion of (1+3x^2)/(1-x-9x^5).
 * https://oeis.org/A098525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 9*a(n-5), a(0) = 1, a(1) = 1, a(2) = 4, a(3) =
 *            4, a(4) = 4. Recurrence valid for all n ≥ 5
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 74, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Simple memoized dynamic array.  No argument checking needed. */
    static seq_t *cache = NULL;      /* cached values */
    static idx_t  size   = 0;         /* allocated size                 */
    static idx_t  last   = 0;         /* first unused index (next to write) */

    /* Initialise cache on first use.  The first 5 terms are known. */
    if (size == 0) {
        size = 8;                      /* initial allocation */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        cache[0] = 1;                  /* a(0) */
        cache[1] = 1;                  /* a(1) */
        cache[2] = 4;                  /* a(2) */
        cache[3] = 4;                  /* a(3) */
        cache[4] = 4;                  /* a(4) */
        last = 5;                      /* next index to compute */
    }

    /* Grow the array if necessary. */
    if (n >= size) {
        idx_t newsize = size * 2;
        if (newsize <= n) newsize = n + 1;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        size = newsize;
    }

    /* Compute any missing terms up to n. */
    while (last <= n) {
        cache[last] = cache[last-1] + (seq_t)9 * cache[last-5];
        ++last;
    }

    return cache[n];
}

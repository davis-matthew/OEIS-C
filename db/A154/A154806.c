/*
 * A154806  Numbers such that every run length in base 2 is 4.
 * https://oeis.org/A154806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(4n+5) + 15*(-1)^(n+1) - 17)/34
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n ≥ 4: a(n) = a(n–2) + 16·a(n–1) – 16·a(n–3) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is the first term.  n==0 is unused. */
    if (n == 0) return (seq_t)0;

    /* Small values can be returned immediately. */
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)240;
    if (n == 3) return (seq_t)3855;

    /* Memoisation via a static cache. */
    static seq_t *cache   = NULL;   /* cache[1] … cache[last] */
    static size_t cap     = 0;      /* allocated size */
    static size_t last   = 3;       /* highest index already computed */

    /* Grow the cache if needed. */
    if (cache == NULL) {
        /* start with space for the first few terms */
        cap = (size_t)n + 1;
        if (cap < 4) cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        /* initialise the base terms */
        cache[0] = (seq_t)0;                 /* unused */
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)240;
        cache[3] = (seq_t)3855;
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n */
    if (last < (size_t)n) {
        for (size_t k = last + 1; k <= (size_t)n; ++k) {
            cache[k] = cache[k-2]
                     + (seq_t)16 * cache[k-1]
                     - (seq_t)16 * cache[k-3];
        }
        last = (size_t)n;
    }

    return cache[n];
}

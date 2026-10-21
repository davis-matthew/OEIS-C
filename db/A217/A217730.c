/*
 * A217730  Expansion of (1+2*x-x^3)/(1-4*x^2+2*x^4).
 * https://oeis.org/A217730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 2*a(n-4), n >= 4 Initial values: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=7
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the n-th term of OEIS sequence A217730. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static size_t  cap   = 0;     /* number of cached entries */

    /* handle the trivial case when nothing is cached yet */
    if (cap == 0) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();           /* insufficient memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)7;
        cap = 4;
    }

    /* extend the cache if the requested index is beyond the current size */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* compute missing values sequentially */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            /* the first four values are already set, so skip them */
            if (i < 4) continue;
            seq_t *ai2 = &cache[i-2];
            seq_t *ai4 = &cache[i-4];
            cache[i] = (*ai2) * (seq_t)4 - (*ai4) * (seq_t)2;
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}

/*
 * A181635  Expansion of 36*x^2*(1+36*x^2-6*x) / ((36*x^2+6*x+1)*(1-6*x)^2).
 * https://oeis.org/A181635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 216*a(n-3) - 1296*a(n-4) for n>4 with initial
 *            values a(1)=0, a(2)=36, a(3)=0, a(4)=1296
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A181635.  */
seq_t a(idx_t n)
{
    /* Cache stores values a(1)…a(cache_size).  Index 0 is unused.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;            /* highest index cached */

    /* Sanity check: offset starts at 1.  */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Initialise the cache lazily with the first four terms.  */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_size + 1));
        if (!cache)
            return (seq_t)0;                /* allocation failure */

        cache[0] = (seq_t)0;                 /* unused */
        cache[1] = (seq_t)0;                 /* a(1) */
        cache[2] = (seq_t)36;                /* a(2) */
        cache[3] = (seq_t)0;                 /* a(3) */
        cache[4] = (seq_t)1296;              /* a(4) */
    }

    /* Return the value if it is already cached.  */
    if (n <= cache_size)
        return cache[n];

    /* Grow the cache to accommodate up to index n.  */
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
    if (!tmp)
        return (seq_t)0;    /* allocation failure */

    cache = tmp;

    /* Compute any missing terms using the recurrence.  */
    for (idx_t i = cache_size + 1; i <= new_size; ++i) {
        if (i == (idx_t)1) {
            cache[i] = (seq_t)0;
        } else if (i == (idx_t)2) {
            cache[i] = (seq_t)36;
        } else if (i == (idx_t)3) {
            cache[i] = (seq_t)0;
        } else if (i == (idx_t)4) {
            cache[i] = (seq_t)1296;
        } else {
            /* Use unsigned arithmetic; the recurrence
               is guaranteed to produce a non‑negative result
               for n>4, so the subtraction never underflows.  */
            seq_t term1 = (seq_t)6 * cache[i - 1];
            seq_t term2 = (seq_t)216 * cache[i - 3];
            seq_t term3 = (seq_t)1296 * cache[i - 4];
            cache[i] = term1 + term2 - term3;
        }
    }
    cache_size = new_size;

    return cache[n];
}

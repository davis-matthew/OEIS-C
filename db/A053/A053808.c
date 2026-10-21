/*
 * A053808  Partial sums of A001891.
 * https://oeis.org/A053808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + (n+1)^2, a(0)=1, a(1)=5
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity of the cache      */
    static idx_t last   = 1;      /* largest index that has been computed */

    /* initialise cache on the first call */
    if (!cache)
    {
        cap   = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)   /* allocation failed – return zero */
            return (seq_t)0;
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)5;   /* a(1) */
        last   = 1;
    }

    /* if the requested term is already known, return it */
    if (n <= last)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cap)
    {
        size_t new_cap = cap == 0 ? 2 : cap;
        while (new_cap <= n)
            new_cap <<= 1;                 /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                          /* allocation failed – return zero */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t term = (seq_t)(i + 1);   /* (i+1)^2 */
        term *= term;
        cache[i] = cache[i - 1] + cache[i - 2] + term;
    }
    last = n;
    return cache[n];
}

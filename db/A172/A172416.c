/*
 * A172416  a(n) = 5*2^n/9 + 1/4 + (-1)^n*(n/6 + 7/36).
 * https://oeis.org/A172416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 2*a(n-4), with initial
 *            values a(0)=1, a(1)=1, a(2)=3, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = (idx_t)-1;  /* last computed index */

    /* ensure the cache is initialized with the first four terms */
    if (cap == 0)
    {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        last = 3;
    }

    /* if the requested index is already cached, just return it */
    if (n <= last)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)n >= cap)
    {
        size_t new_cap = cap < 2 ? 2 : cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute the missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* a(i) = a(i-1) + 3*a(i-2) - a(i-3) - 2*a(i-4) */
        seq_t val = cache[i-1];
        val += cache[i-2] * (seq_t)3;
        val -= cache[i-3];
        val -= cache[i-4] * (seq_t)2;
        cache[i] = val;
    }

    last = n;
    return cache[n];
}

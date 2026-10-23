/*
 * A190089  Row sums of the triangular matrix A190088.
 * https://oeis.org/A190089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=5*a(n-1)+2*a(n-2)+a(n-3); a(0)=1, a(1)=4, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms         */
    static size_t cap   = 0;      /* current allocated capacity      */
    static idx_t  last  = (idx_t)-1; /* highest index that has been computed */

    /* first call: initialise the cache with the seed values */
    if (last == (idx_t)-1) {
        /* we always need at least the first three terms */
        cap = 3;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;       /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)21;
        last   = 2;                /* we have computed indices 0..2 */
    }

    /* enlarge cache as necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 3;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;        /* double until big enough */

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;       /* reallocation failed */

        cache = tmp;
        cap   = new_cap;
    }

    /* if n <= 2 we already have the value */
    if (n <= 2)
        return cache[n];

    /* compute missing terms up to n using the recurrence */
    if (last < n) {
        /* the recurrence uses a(i-1), a(i-2), a(i-3) so the
           first new index that can be computed is 3.  Since we
           have values for indices 0..2, we can safely start
           from max(3, last+1).  In practice last will always be
           >=2 after the first initialisation, so start = last+1. */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            cache[i] = a1 * (seq_t)5 + a2 * (seq_t)2 + a3;
        }
        last = n;
    }

    return cache[n];
}

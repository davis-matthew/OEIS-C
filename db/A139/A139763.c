/*
 * A139763  a(n)=a(n-1)+a(n-2)+a(n-3)+2a(n-4) with a(n)=n+1 for n<=3.
 * https://oeis.org/A139763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)+a(n-2)+a(n-3)+2a(n-4) with a(n)=n+1 for n<=3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: lazily allocated and expanded. */
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* allocated capacity */
    static idx_t max_computed = 0;  /* highest index already computed */

    if (cache == NULL) {            /* first call: allocate and store the initial values */
        size = 4;
        cache = (seq_t *)malloc((size_t)size * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;        /* allocation failure – fall back to zero */

        cache[0] = (seq_t)1;        /* a(0) */
        cache[1] = (seq_t)2;        /* a(1) */
        cache[2] = (seq_t)3;        /* a(2) */
        cache[3] = (seq_t)4;        /* a(3) */
        max_computed = 3;
    }

    /* Ensure the cache is large enough to hold index n */
    if (n + (idx_t)1 > size) {
        idx_t newsize = size ? size * 2 : 4;
        while (newsize <= n) newsize <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)newsize * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;        /* allocation failure */
        cache = tmp;
        /* initialise the newly allocated slots to 0 */
        for (idx_t i = size; i < newsize; ++i)
            cache[i] = (seq_t)0;
        size = newsize;
    }

    /* If the requested value is already computed, simply return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute new values up to n */
    for (idx_t i = max_computed + (idx_t)1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3]
                 + ((seq_t)2 * cache[i - 4]);
    }
    max_computed = n;
    return cache[n];
}

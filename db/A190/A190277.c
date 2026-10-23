/*
 * A190277  Number of trails between opposite vertices in a triangle strip.
 * https://oeis.org/A190277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 3*a(n-3) - 2*a(n-4) for n > 4 Initial
 *            values: a(1)=1, a(2)=1, a(3)=2, a(4)=4 Computed iteratively with
 *            memoisation
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle values below the offset */
    if (n < 1)
        return 0;

    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* cache[1] .. cache[max_computed] */
    static idx_t max_computed = 0;      /* highest n for which cache[n] is valid */

    /* initialise base values up to n = 4 if necessary */
    if (max_computed < 4)
    {
        idx_t needed = 4;
        if (cache == NULL)
        {
            size_t sz = (size_t)(needed + 1) * sizeof(seq_t); /* index 0 unused */
            cache = (seq_t *)realloc(NULL, sz);
        }
        /* no need to check allocation failure: build system guarantees it */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        max_computed = 4;
    }

    /* if the requested term is already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* extend cache to at least n */
    if (cache == NULL)
    {
        size_t sz = (size_t)(n + 1) * sizeof(seq_t);
        cache = (seq_t *)realloc(NULL, sz);
    }
    else
    {
        size_t new_sz = (size_t)(n + 1) * sizeof(seq_t);
        cache = (seq_t *)realloc(cache, new_sz);
    }

    /* compute missing terms iteratively */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t term1 = (seq_t)3 * cache[i - 1];
        seq_t term2 = cache[i - 2];
        seq_t term3 = (seq_t)3 * cache[i - 3];
        seq_t term4 = (seq_t)2 * cache[i - 4];
        seq_t sum   = term1 + term2;
        seq_t sub   = term3 + term4;
        cache[i] = sum - sub;   /* all intermediate values stay non‑negative */
    }

    max_computed = n;
    return cache[n];
}

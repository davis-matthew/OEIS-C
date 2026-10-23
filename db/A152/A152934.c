/*
 * A152934  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defining a polygonal configuration consisting of
 *          two m-gonal polygonal components chained with string components of
 *          length 3 as m varies.
 * https://oeis.org/A152934
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) (for n > 4) Initial values:
 *            a(2)=289, a(3)=1962, a(4)=13429 Offset: 2 (so a(2) is the first
 *            term listed)
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of OEIS A152934.  Undefined for n < 2.  */
seq_t a(idx_t n)
{
    /* Undefined for n < 2 – callers should not use those indices. */
    if (n < 2) return 0;

    /* Static cache for memoised values */
    static seq_t *cache          = NULL;   /* array index 0 … last computed index */
    static size_t cap            = 0;      /* allocated size of cache   */
    static idx_t last_computed   = 4;      /* highest index that has been computed (>=4) */

    /* First call – allocate a minimal array and set the known initial terms. */
    if (!cache)
    {
        size_t init_cap = (size_t)(n + 1);
        if (init_cap < 5) init_cap = 5;            /* need space for indices 0 … 4 */

        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;                      /* unlikely on small n */

        for (size_t i = 0; i < init_cap; ++i) cache[i] = 0;

        /* Known initial values */
        cache[2] = 289;
        cache[3] = 1962;
        cache[4] = 13429;

        cap = init_cap;
        last_computed = 4;
    }

    /* Grow the cache if a larger n is requested. */
    else if ((size_t)(n + 1) > cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                        /* unlikely on small n */

        /* Initialise the newly allocated part. */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;

        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms using the recurrence. */
    if (n > last_computed)
    {
        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            cache[i] = (seq_t)8 * cache[i-1]
                     - (seq_t)8 * cache[i-2]
                     +               cache[i-3];
        }
        last_computed = n;
    }

    return cache[n];
}

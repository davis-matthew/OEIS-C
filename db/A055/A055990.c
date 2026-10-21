/*
 * A055990  a(n) is its own 4th difference.
 * https://oeis.org/A055990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-6*a(n-2)+4*a(n-3)-a(n-4) Initial values: a(1)=1,
 *            a(2)=4, a(3)=14, a(4)=50
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms (0‑based index → a(n+1)) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of cached terms */
    static int init = 0;           /* flag whether first‑time initialisation is done */

    if (!init)
    {
        /* Allocate space for the first four terms and initialise them */
        cache = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
        cache[0] = 1;   /* a(1) */
        cache[1] = 4;   /* a(2) */
        cache[2] = 14;  /* a(3) */
        cache[3] = 50;  /* a(4) */
        cache_size = 4;
        init = 1;
    }

    /* If the requested index is already cached, return it */
    if (n <= (idx_t)cache_size)
        return cache[n - 1];

    /* Grow the cache to accommodate up to n */
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);          /* fatal if allocation fails */
    cache = tmp;

    /* Compute missing terms from cache_size+1 up to n */
    for (size_t i = cache_size; i < new_size; ++i)
    {
        /* i is the 0‑based index of a(i+1); need last four terms */
        seq_t term = 5 * cache[i - 1]
                   - 6 * cache[i - 2]
                   + 4 * cache[i - 3]
                   - cache[i - 4];
        cache[i] = term;
    }
    cache_size = new_size;
    return cache[n - 1];
}

/*
 * A098260  Chebyshev polynomials S(n,627).
 * https://oeis.org/A098260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A098260:  a(n) = 627*a(n-1) - a(n-2)  with a(0)=1, a(1)=627 */

/* The recurrence can be evaluated iteratively.  To avoid repeated
 * recomputation when a(n) is requested in arbitrary order, a static
 * cache is maintained and extended as necessary.  All arithmetic is
 * performed with the type seq_t, so the code works with 8‑, 16‑, 32‑,
 * 64‑, and 128‑bit integers as well as arbitrary‑precision implementations. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached values a(0) … a(k) */
    static idx_t cache_size = 0;         /* number of stored elements */

    /* extend the cache if the requested index is not yet available */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;  /* out of memory – unlikely in the test harness */

        /* copy existing cached values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* initialise base cases if they are missing */
        if (cache_size == 0)
        {
            new_cache[0] = 1;
            cache_size = 1;
        }
        if (cache_size == 1 && new_size > 1)
        {
            new_cache[1] = 627;
            cache_size = 2;
        }

        /* compute any additional terms up to n */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            /* recurrence requires at least two previous terms */
            if (i == 0)
                new_cache[0] = 1;
            else if (i == 1)
                new_cache[1] = 627;
            else
                new_cache[i] = 627 * new_cache[i - 1] - new_cache[i - 2];
        }

        cache_size = new_size;
        free(cache);
        cache = new_cache;
    }

    return cache[n];
}

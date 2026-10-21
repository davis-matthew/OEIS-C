/*
 * A099366  Squares of A005668.
 * https://oeis.org/A099366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*a(n-1) - a(n-2) - 2*(-1)^n Base values: a(0)=0, a(1)=1
 *            (n ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The recursive (caching) implementation works for any seq_t type
   (built‑in integer, unsigned __int128, or arbitrary‑precision
   class).  All arithmetic uses the operators provided by seq_t. */

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache       = NULL;   /* array of terms                 */
    static size_t cache_cap  = 0;        /* allocated capacity in elements  */
    static size_t cache_len  = 0;        /* number of valid terms in cache  */

    /* Initialisation: ensure the cache contains a(0) and a(1) */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure: return 0 */

        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* Grow the cache if we need more space */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;  /* double */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failure */

        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to the requested index if necessary */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) { cache[0] = 0; continue; }
        if (i == 1) { cache[1] = 1; continue; }

        seq_t prev     = cache[i - 1];
        seq_t prevprev = cache[i - 2];
        /* (-1)^i = 1 for even i, -1 for odd i */
        int factor = ((i % 2) == 0) ? 1 : -1;
        seq_t val = 38 * prev - prevprev - (seq_t)(2 * factor);
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;   /* we now have terms[0..n] */

    return cache[(size_t)n];
}

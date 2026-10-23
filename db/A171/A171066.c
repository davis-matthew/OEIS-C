/*
 * A171066  Expansion of g.f.: x*(1 - x)*(1 + x)/(1 - x - 9*x^2 - x^3 + x^4).
 * https://oeis.org/A171066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 9*a(n-2) + a(n-3) - a(n-4); Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values.  Works for any seq_t (fixed or arbitrary‑precision). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* Number of elements stored (>= 0) */

/* Return a(n) for any n >= 0, memoizing results in static cache. */
seq_t a(idx_t n)
{
    /* Initialise cache with first four terms on first call. */
    if (!cache)
    {
        cache_size = 4;   /* we will store indices 0..3 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;     /* allocation failure → undefined value */

        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 9;
    }

    /* If the requested n is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to accommodate indices up to n. */
    size_t new_size = (size_t)n + 1;            /* array size needed */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;      /* allocation failure → undefined value */
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t val = cache[i-1] +
                    ((seq_t)9 * cache[i-2]) +
                    cache[i-3];
        val -= cache[i-4];
        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n];
}

/*
 * A071281  Numerators of Peirce sequence of order 3.
 * https://oeis.org/A071281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2*a(n-6) - a(n-12) for n > 11, with initial values
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=2, a(6)=2, a(7)=3,
 *            a(8)=1, a(9)=4, a(10)=3, a(11)=5
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for previously computed values */
static seq_t *cache = NULL;      /* array of computed terms          */
static idx_t cache_size = 0;     /* allocated length of cache array   */
static idx_t cache_filled = 0;   /* highest index computed + 1       */

/* Ensure cache has at least 'needed+1' elements, reallocating as necessary */
static void grow_cache(idx_t needed)
{
    if (needed < cache_size)
        return;
    idx_t newsize = cache_size ? cache_size * 2 : 16;
    if (newsize <= needed)
        newsize = needed + 1;
    cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    cache_size = newsize;
}

/* Compute terms up to 'upto' (non‑inclusive) and fill cache */
static void compute_to(idx_t upto)
{
    if (upto < cache_filled)
        return;                     /* nothing to do */

    grow_cache(upto);

    /* Initial values for indices 0..11 */
    static const seq_t init[12] = {
        0, 0, 0, 1, 1, 2, 2, 3, 1, 4, 3, 5
    };
    for (idx_t i = cache_filled; i < upto && i < 12; ++i)
        cache[i] = init[i];

    /* Use recurrence for i >= 12 */
    for (idx_t i = cache_filled; i < upto; ++i) {
        if (i < 12)           /* already initialized above */
            continue;
        seq_t val = cache[i - 6];
        val = val + val;      /* 2 * a(i-6) */
        val -= cache[i - 12]; /* 2*a(i-6) - a(i-12) */
        cache[i] = val;
    }

    cache_filled = upto;
}

/* Externally visible function required by the build system */
seq_t a(idx_t n)
{
    /* Compute (and cache) all terms up to index n */
    compute_to(n + 1);
    return cache[n];
}

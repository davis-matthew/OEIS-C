/*
 * A063135  Dimension of the space of weight 2n cusp forms for Gamma_0( 67 ).
 * https://oeis.org/A063135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 5 a(1) = 5, a(2) = 16,
 *            a(3) = 28, a(4) = 38, a(5) = 50 (All indices are 1‑based; values
 *            for n ≤ 0 are undefined.)
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache     = NULL;   /* 1‑based array of computed values   */
static size_t cache_len = 0;      /* highest index currently stored    */
static size_t cache_cap = 0;      /* allocated capacity (≥ cache_len+1)*/

/* Ensure that the cache can hold at least the indices 0 … needed-1. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise the cache with the first five terms of the sequence. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* Allocate space for indices 0 … 5 (index 0 is unused). */
    cache_cap = 16;                      /* arbitrary but sufficient start */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 5;                       /* highest stored index */
    cache[0] = (seq_t)0;                 /* unused placeholder */
    cache[1] = (seq_t)5;                 /* a(1) */
    cache[2] = (seq_t)16;                /* a(2) */
    cache[3] = (seq_t)28;                /* a(3) */
    cache[4] = (seq_t)38;                /* a(4) */
    cache[5] = (seq_t)50;                /* a(5) */
}

/* Return a(n) for the OEIS A063135 sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* not defined, but harmless return */

    if (cache == NULL)
        init_cache();

    if (n <= cache_len)
        return cache[n];

    /* Make sure the array is large enough. */
    ensure_cache_cap((size_t)n + 1);

    /* Compute terms up to n using the recurrence relation. */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k)
    {
        seq_t val = cache[k - 1] + cache[k - 3] - cache[k - 4];
        cache[k] = val;
    }

    cache_len = (size_t)n;
    return cache[n];
}

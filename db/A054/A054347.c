/*
 * A054347  Partial sums of A000201.
 * https://oeis.org/A054347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 1167, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A054347 – Partial sums of floor(k*φ) where φ = (1+√5)/2
 *
 *  Formula used:
 *      a(n) = Σ_{k=0..n} ⌊ k·φ ⌋
 *  Since   ⌊ k·φ ⌋ = ⌊ (k + ⌊k·√5⌋) / 2 ⌋   for k ≥ 0,
 *  each term can be computed with integer arithmetic only:
 *      term(k) = (k + floor_sqrt(5·k²)) / 2
 *
 *  The function keeps a cache of the partial sums; on each call it
 *  extends the cache up to the requested index, computing each new
 *  a(i) consecutively.  Consequently any sequence of calls – the
 *  first increasing, the first decreasing or arbitrary – runs in
 *  O(max_index) time overall.
 *
 *  All arithmetic is performed in the build‑specific integer type
 *  seq_t; idx_t is the index type (uint64_t).  An auxiliary
 *  integer‑square‑root routine `floor_sqrt` is provided; it works
 *  for any seq_t that supports +, -, *, / and %.
 */

#include <stdlib.h>

/* Integer square root – works for any seq_t that supports +, -, *, / and %. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;          /* √x ≤ x for x≥0 */
    seq_t ans = 0;

    while (lo <= hi)
    {
        seq_t mid  = (lo + hi) / 2;
        seq_t mid2 = mid * mid;

        if (mid2 <= x)
        {
            ans = mid;
            lo  = mid + 1;
        }
        else
        {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Cache of the computed partial sums a(i), 0 ≤ i < cache_size. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;      /* number of entries currently stored */

/* Computes all a(i) for i = cache_size .. n, extending the cache. */
static void extend_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_size)   /* already present */
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)   /* allocation failed – abort with 0 */
        return;

    /* initialise first value if cache was empty */
    if (cache_size == 0)
    {
        new_cache[0] = (seq_t)0;
        cache_size = 1;
    }

    /* compute new terms */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        seq_t ii   = (seq_t)i;
        seq_t ksq  = ii * ii;          /* i² */
        seq_t sq5  = floor_sqrt((seq_t)5 * ksq);   /* ⌊i·√5⌋ */
        seq_t term = (ii + sq5) / (seq_t)2;        /* ⌊i·φ⌋ */

        new_cache[i] = new_cache[i-1] + term;      /* partial sum */
    }

    cache      = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    if ((size_t)n < cache_size)
        return cache[(size_t)n];
    /* if allocation failed earlier, return 0 as a sentinel */
    return (seq_t)0;
}

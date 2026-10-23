/*
 * A016203  Expansion of g.f. 1/((1-x)*(1-2*x)*(1-8*x)).
 * https://oeis.org/A016203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fa[n] with * a(0) = 1, * a(n) = 8*a(n-1) + 2^(n+1) - 1 (n
 *            >= 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for memoisation */
static seq_t *cache = NULL;     /* array of computed terms   */
static size_t cached_cap = 0;   /* current capacity (bytes)  */
static idx_t  cached_max  = 0;  /* highest index known       */
static seq_t  pow2_cache  = 0;  /* current value of 2^(k+1)  */

/*  Compute a(n) using the linear recurrence and memoisation  */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!cache) {
        cached_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache) return 0;          /* unlikely to happen */
        cache[0] = 1;                 /* a(0) = 1              */
        cached_max = 0;
        pow2_cache = 2;               /* 2^(0+1) = 2           */
    }
    /* Expand the cache if necessary */
    else if ((size_t)n + 1 > cached_cap) {
        cached_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
        if (!cache) return 0;          /* again: unlikely */
    }

    /* Return cached value if already computed */
    if (n <= cached_max)
        return cache[n];

    /* Compute terms from cached_max+1 up to n  */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        pow2_cache <<= 1;                               /* pow2 = 2^(i+1)       */
        cache[i] = cache[i - 1] * 8 + pow2_cache - 1;    /* recurrence          */
    }
    cached_max = n;
    return cache[n];
}

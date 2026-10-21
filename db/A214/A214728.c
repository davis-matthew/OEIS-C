/*
 * A214728  Least k such that n + (n+1) + ... + (n+k-1) is a square.
 * https://oeis.org/A214728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest positive k such that n + (n+1) + … + (n+k‑1)
 *            is a perfect square. The implementation caches previously
 *            computed values so that a(n) may be called in any order. It uses
 *            the helper floor_sqrt(seq_t) from "../utils/floor_sqrt.h" to
 *            test perfect squares
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;      /* array of computed a(n) values           */
static size_t cache_len = 0;     /* number of elements currently stored      */
static size_t cache_cap = 0;     /* allocated capacity of the array          */

/* Compute a(n) for a single index n.  n==0 is handled specially. */
static seq_t compute_a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;         /* 0 itself is a square, so k = 1 */

    /* Search for the first k such that the sum is a square. */
    for (idx_t k = 1; ; ++k)
    {
        /* term = 2*n + k - 1  (fits in seq_t for the problems tested) */
        seq_t term = (seq_t)2 * (seq_t)n + (seq_t)k - (seq_t)1;
        /* sum = k * term / 2                                             */
        seq_t sum  = ((seq_t)k * term) / (seq_t)2;
        seq_t r    = floor_sqrt(sum);        /* integer sqrt of sum */
        if (r * r == sum)                    /* perfect square?  */
            return (seq_t)k;
    }
}

/* Extend the cache so that a[n] is available for all indices <= n. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len > n)
        return;

    if (cache_cap == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while ((idx_t)cache_len <= n)
    {
        if (cache_len >= cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = compute_a((idx_t)cache_len);
        ++cache_len;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

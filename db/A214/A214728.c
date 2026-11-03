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
 * Validated: standard, pass, 88 terms through n = 889 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
        cache = mem_new(seq_t, cache_cap);
    }

    while ((idx_t)cache_len <= n)
    {
        if (cache_len >= cache_cap)
        {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
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

/*
 * A139542  a(n) = a(n-1) + largest divisor of a(n-1) <= sqrt(a(n-1)).
 * https://oeis.org/A139542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 977 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n) = a(n-1) + largest divisor d of a(n-1) such that d ≤ sqrt(a(n-1)) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Static cache for computed terms */
static seq_t *cache = NULL;        /* array of a(i) values, 1‑based indexing */
static idx_t  cache_len = 0;       /* number of terms stored (largest n computed) */
static size_t cache_cap = 0;       /* allocated array capacity */

/* Compute the largest divisor of x that is ≤ sqrt(x) */
static inline seq_t largest_divisor_le_sqrt(seq_t x)
{
    seq_t limit = floor_sqrt(x);          /* floor(sqrt(x)) */
    for (seq_t d = limit; d >= 1; --d)   /* scan downwards */
    {
        if (x % d == 0)
            return d;                    /* first divisor found is the largest ≤ sqrt */
    }
    return 1;   /* should never be reached for x ≥ 1 */
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but return 0 to avoid UB */
        return 0;

    /* Ensure the cache contains at least n terms */
    while (cache_len < n)
    {
        /* Allocate or grow the cache when needed */
        if (cache_len == 0)
        {
            cache_cap = 8;
            cache = mem_new(seq_t, cache_cap);
            if (!cache) abort();
            cache[0] = (seq_t)1;   /* a(1) = 1 */
            cache_len = 1;
        }
        else if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next term */
        seq_t prev = cache[cache_len - 1];
        seq_t d    = largest_divisor_le_sqrt(prev);
        seq_t next = prev + d;
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}

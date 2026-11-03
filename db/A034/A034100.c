/*
 * A034100  Fractional part of square root of a(n) starts with digit 4.
 * https://oeis.org/A034100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth integer k such that floor(sqrt(k)) + 0.4 ≤
 *            sqrt(k) < floor(sqrt(k)) + 0.5 (i.e. the fractional part of
 *            sqrt(k) starts with digit 4)
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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

/* static cache for the sequence */
static seq_t *cache = NULL;      /* array of calculated a(i) values */
static size_t cache_cap = 0;     /* allocated capacity of the array */
static size_t cache_len = 0;     /* number of stored terms */
static uint64_t last_checked = 0;/* largest integer k examined (0 …) */

/* enlarge the cache array to hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* a(idx_t n) returns the nth term of the sequence, 1‑based.  */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1 – return 0 harmlessly */
        return 0;

    /* ensure the cache has at least n values */
    while (cache_len < n) {
        ++last_checked;      /* next integer to test */

        /* floor(sqrt(last_checked)) */
        seq_t f = floor_sqrt((seq_t)last_checked);

        /* (20*f + 8)^2  and  (20*f + 10)^2  are the bounds for 400*k
           (they fit into unsigned __int128 for all practical n). */
        unsigned __int128 s = (unsigned __int128)f;
        unsigned __int128 L = (20 * s + 8) * (20 * s + 8);
        unsigned __int128 R = (20 * s + 10) * (20 * s + 10);
        unsigned __int128 dbl = (unsigned __int128)last_checked * 400;

        if (L <= dbl && dbl < R) {
            /* store the qualifying integer in the cache */
            if (cache_len == cache_cap)
                grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    /* cache is 0‑based; nth term is at index n‑1 */
    return cache[n - 1];
}

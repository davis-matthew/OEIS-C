/*
 * A159022  a(0)=29; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0.
 * https://oeis.org/A159022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=29; a(n)=a(n‑1)+floor(sqrt(a(n‑1))), n>0
 * Range:     u8 n <= 6, u16 n <= 183, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 901 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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

static seq_t *cache = NULL;      /* dynamic array of computed terms   */
static idx_t  cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity in elements     */

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* Allocate at least one extra to avoid repeated tiny reallocs */
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;                 /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first element if this is the first call */
    if (cache_len == 0) {
        cache[0] = 29;            /* a(0) */
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    while (cache_len <= n) {
        seq_t prev = cache[cache_len - 1];
        seq_t inc  = floor_sqrt(prev);        /* floor(sqrt(prev)) */
        cache[cache_len] = prev + inc;
        cache_len++;
    }

    return cache[n];
}

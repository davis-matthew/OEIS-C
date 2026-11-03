/*
 * A159258  a(0)=155; for n > 0, a(n) = a(n-1) + floor(sqrt(a(n-1))).
 * https://oeis.org/A159258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor_sqrt(a(n-1)), a(0) = 155
 * Range:     u8 n <= 0, u16 n <= 49, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 66 terms through n = 4679 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    const size_t target = (size_t)n;

    /* initialise cache for a(0) */
    if (cap == 0) {
        cap = 1;
        cache = mem_new(seq_t, cap);
        cache[0] = (seq_t)155;
        len = 1;
    }

    /* grow the array if necessary */
    if (target >= cap) {
        size_t newcap = cap;
        while (target >= newcap) newcap = newcap ? newcap << 1 : 1;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    if (target >= len) {
        for (size_t i = len; i <= target; ++i) {
            seq_t prev = cache[i - 1];
            seq_t inc  = floor_sqrt(prev);
            cache[i]   = prev + inc;
        }
        len = target + 1;
    }

    return cache[target];
}

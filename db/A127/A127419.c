/*
 * A127419  Recurrence: a(n) = a(n-1) + floor( (sqrt(8 * a(n-1) - 7) - 1)/2 )
 *          for n>=2 with a(0)=1, a(1)=2.
 * https://oeis.org/A127419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(n)=a(n-1)+floor((sqrt(8*a(n-1)-7)-1)/2) for
 *            n>=2
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 986 (2026-11-03)
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

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of slots currently allocated (max index+1) */

    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* grow cache if necessary */
    if ((idx_t)(n + 1) > cache_size) {
        idx_t new_size = (idx_t)(n + 1);
        if (cache == NULL) {
            cache = mem_new(seq_t, new_size);
        } else {
            cache = mem_resize(seq_t, cache, new_size);
        }
        if (cache == NULL) {
            /* Allocation failed; return 0 as sentinel (should never happen in tests) */
            return (seq_t)0;
        }
        /* Fill new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)2;
            } else {
                /* compute recurrence */
                seq_t prev = cache[i - 1];
                seq_t t = prev * (seq_t)8 - (seq_t)7;
                seq_t r = floor_sqrt(t);                /* floor(sqrt(t)) */
                seq_t d = (r - (seq_t)1) / (seq_t)2;     /* floor((r-1)/2) */
                cache[i] = prev + d;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}

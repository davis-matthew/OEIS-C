/*
 * A031553  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 55.
 * https://oeis.org/A031553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the continued fraction for sqrt(k) has even period
   and, after deleting the last term of the periodic part, the central
   (m‑th) term equals 55. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static state to allow arbitrary order of calls */
    static seq_t *cache          = NULL;   /* stored sequence values   */
    static size_t cache_cap      = 0;       /* allocated capacity in cache */
    static size_t cache_len      = 0;       /* number of values filled   */

    static seq_t *period         = NULL;   /* buffer for one period   */
    static size_t period_cap     = 0;       /* allocated capacity in period buffer */
    static size_t period_len     = 0;       /* current number of terms in period */

    static seq_t next_candidate  = 2;       /* next integer to test    */

    /* Continue generating terms until we have at least n terms */
    while (cache_len < n)
    {
        seq_t d = next_candidate++;               /* potential candidate   */
        seq_t a0 = floor_sqrt(d);                 /* integer part of sqrt  */
        if (a0 * a0 == d)                         /* skip perfect squares */
            continue;

        /* Compute continued fraction of sqrt(d) */
        seq_t m = 0, dd = 1, a = a0;
        period_len = 0;                           /* reset period buffer   */

        while (true)
        {
            /* advance to next triple (m,dd,a) */
            m  = dd * a - m;
            dd = (d - m * m) / dd;
            a  = (a0 + m) / dd;

            /* store term a in the period buffer */
            if (period_len == period_cap) {
                size_t old_cap = period_cap;
                period_cap = period_cap ? period_cap * 2 : 1;
                period = seq_realloc(period, old_cap, period_cap);
            }
            period[period_len++] = a;

            if (dd == 1)   /* period ends when denominator becomes 1   */
                break;
        }

        /* Check the required property */
        if ((period_len & 1) == 0) {                /* even period only     */
            seq_t central = period[period_len / 2 - 1];
            if (central == (seq_t)55) {
                /* store d into the result cache */
                if (cache_len == cache_cap) {
                    size_t old_cap = cache_cap;
                    cache_cap = cache_cap ? cache_cap * 2 : 1;
                    cache = seq_realloc(cache, old_cap, cache_cap);
                }
                cache[cache_len++] = d;
            }
        }
    }

    /* Return the n-th term (offset 1) */
    return cache[n - 1];
}

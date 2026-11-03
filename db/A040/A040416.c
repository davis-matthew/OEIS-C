/*
 * A040416  Continued fraction for sqrt(437).
 * https://oeis.org/A040416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
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

/* Continued fraction for sqrt(437):
   a(0) = floor(sqrt(437)),
   a(n) = period[(n-1) mod 6] for n ≥ 1,
   period = {1, 9, 2, 9, 1, 40}. */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(437). */
    static const seq_t period[] = {1, 9, 2, 9, 1, 40};
    const size_t period_len = sizeof period / sizeof period[0];

    if (n == 0) {
        /* The first term is the integer part of sqrt(437). */
        return floor_sqrt((seq_t)437);
    }

    /* n-1 is guaranteed non‑negative because n > 0 here. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}

/*
 * A040619  Continued fraction for sqrt(645).
 * https://oeis.org/A040619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040619: continued fraction terms of √645.
   Computed by the standard algorithm for quadratic surds. */
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

seq_t a(idx_t n)
{
    /* D is the radicand */
    const seq_t D = (seq_t)645;
    /* Integer part of the square root (a₀) */
    const seq_t a0 = floor_sqrt(D);

    if (n == 0)
        return a0;   /* a(0) = ⌊√645⌋ */

    /* Variables for the continued‑fraction algorithm */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    /* Iterate to the desired index */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t m2 = d * a - m;
        seq_t d2 = (D - m2 * m2) / d;
        seq_t a2 = (a0 + m2) / d2;

        m = m2;
        d = d2;
        a = a2;
    }
    return a;
}

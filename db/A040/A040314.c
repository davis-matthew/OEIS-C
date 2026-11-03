/*
 * A040314  Continued fraction for sqrt(333).
 * https://oeis.org/A040314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 74 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction of sqrt(333).  The first term a(0) is floor(sqrt(333)),
   and for n ≥ 1 the standard square‑root continued‑fraction recurrence
   is used:  
   m_n   = d_{n-1} * a_{n-1} – m_{n-1}
   d_n   = (333 – m_n²) / d_{n-1}
   a_n   = (18 + m_n) / d_n   (here a0 = 18)   */
#include <stdint.h>
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

seq_t a(idx_t n)
{
    const idx_t D = 333U;
    seq_t a0 = (seq_t)floor_sqrt((seq_t)D);
    if (n == 0) return a0;

    seq_t m = 0, d = 1, a1 = a0;
    for (idx_t i = 1; i <= n; ++i) {
        m = d * a1 - m;
        d = ((seq_t)D - m * m) / d;
        a1 = (a0 + m) / d;
    }
    return a1;
}

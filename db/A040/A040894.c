/*
 * A040894  Continued fraction for sqrt(925).
 * https://oeis.org/A040894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   cont‐fraction terms of sqrt(925)
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 84 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(925)) = 30 */
/* a(n) for n>0 are computed by the standard recurrence for regular
   continued fractions of √N with N = 925. */

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
    /* N is the non‑square integer for which we take the continued fraction. */
    const seq_t N = 925;
    /* Leading term of the continued fraction. */
    seq_t a0 = floor_sqrt(N);

    if (n == 0)
        return a0;

    /* Initial values for the recurrence. */
    seq_t m   = 0;     /* m₀ */
    seq_t d   = 1;     /* d₀ */
    seq_t a_k = a0;    /* a₀ */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute the (k+1)‑th triple from the k‑th one. */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Advance to the next step. */
        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }

    return a_k;
}

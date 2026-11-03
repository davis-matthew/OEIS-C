/*
 * A042384  Numerators of continued fraction convergents to sqrt(719).
 * https://oeis.org/A042384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 181 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(719) */

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
    const seq_t D   = (seq_t)719;          /* radicand of the sqrt */
    seq_t a0        = floor_sqrt(D);        /* a₀ = ⌊√719⌋ = 26 */

    seq_t m        = (seq_t)0;
    seq_t d        = (seq_t)1;
    seq_t a_cur    = a0;                     /* aᵢ for current step */

    seq_t p_prev2  = (seq_t)0;             /* p_{-2} */
    seq_t p_prev1  = (seq_t)1;             /* p_{-1} */
    seq_t p        = (seq_t)0;             /* current numerator */

    for (idx_t i = 0;; ++i) {
        p = a_cur * p_prev1 + p_prev2;     /* pᵢ = aᵢ·p_{i‑1} + p_{i‑2} */

        if (i == n)
            return p;                       /* a(n) = numerator of n‑th convergent */

        /* compute next continued‑fraction coefficient a_{i+1} */
        seq_t m_next   = d * a_cur - m;
        seq_t m_next2  = m_next * m_next;
        seq_t d_next   = (D - m_next2) / d;
        seq_t a_next   = (a0 + m_next) / d_next;

        /* update for next iteration */
        m      = m_next;
        d      = d_next;
        a_cur  = a_next;

        /* shift recurrence values */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
}

/*
 * A042334  Numerators of continued fraction convergents to sqrt(694).
 * https://oeis.org/A042334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(694)
 * Range:     u8 none, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 189 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* 694 is the quadratic surd being approximated */
    seq_t D = (seq_t)694;
    seq_t a0 = floor_sqrt(D);

    if (n == 0)
        return a0;

    /* Continued‑fraction data */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_coef = a0;

    /* Convergent recurrence p_{-2}=0, p_{-1}=1 */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_k = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k == 0) {
            a_coef = a0;
        } else {
            /* Compute next m, d, a using the standard algorithm */
            seq_t m_next = d * a_coef - m;
            seq_t d_next = D - m_next * m_next;
            d_next = d_next / d;           /* integer division */
            m = m_next;
            d = d_next;
            a_coef = (a0 + m) / d;
        }

        /* Convergent numerator */
        p_k = a_coef * p_prev1 + p_prev2;
        if (k == n)
            return p_k;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_k;
    }

    /* Unreachable but keeps compiler happy */
    return p_k;
}

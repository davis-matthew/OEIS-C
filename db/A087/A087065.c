/*
 * A087065  Sum{Floor(n*r^k): r=sqrt(3)-1, k>=0}.
 * https://oeis.org/A087065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087065: a(n) = sum_{k>=0} floor(n * (sqrt(3)-1)^k) */

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
    seq_t sum = 0;
    /* coefficients for r^k = a_k + b_k * sqrt(3)   */
    int64_t a_k = 1;   /* r^0 = 1  */
    int64_t b_k = 0;   /* r^0 = 1  */
    for (;;) {
        /* m = n * b_k  (may be negative) */
        int64_t m = (int64_t)n * b_k;
        /* compute floor(m * sqrt(3)) */
        seq_t term_floor_part;
        if (m >= 0) {
            seq_t m_abs = (seq_t)m;                     /* positive */
            seq_t tmp = m_abs * m_abs;                  /* m^2   */
            seq_t tmp3 = tmp * (seq_t)3;                /* 3*m^2 */
            seq_t f = floor_sqrt(tmp3);                /* floor(sqrt(3)*m) */
            term_floor_part = f;
        } else {
            int64_t m_abs_i = -m;
            seq_t m_abs = (seq_t)m_abs_i;               /* positive */
            seq_t tmp = m_abs * m_abs;                  /* m^2   */
            seq_t tmp3 = tmp * (seq_t)3;                /* 3*m^2 */
            seq_t f = floor_sqrt(tmp3);                /* floor(sqrt(3)*|m|) */
            /* floor(m*sqrt3) = -(floor(|m|*sqrt3) + 1) for negative m */
            term_floor_part = -(f + (seq_t)1);
        }

        /* term = n*a_k + floor(m*sqrt3) */
        seq_t term = (seq_t)n * (seq_t)a_k + term_floor_part;
        if (term == 0)
            break;
        sum += term;

        /* next coefficients: (a_{k+1}, b_{k+1}) = (3*b_k - a_k, a_k - b_k) */
        int64_t a_next = 3 * b_k - a_k;
        int64_t b_next = a_k - b_k;
        a_k = a_next;
        b_k = b_next;
    }
    return sum;
}

/*
 * A041938  Numerators of continued fraction convergents to sqrt(492).
 * https://oeis.org/A041938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n‑th convergent to √492 (obtained from
 *            the standard continued fraction expansion of √492)
 * Range:     u8 none, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 198 (2026-11-01)
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
    /* Constants for D = 492 */
    const seq_t D   = (seq_t)492;
    const seq_t a0  = floor_sqrt(D);   /* floor(√492) = 22 */

    /* Variables for the continued‑fraction recurrence (m, d, a) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = a0;                     /* current partial quotient */

    /* Convergent numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute the k‑th numerator */
        seq_t p_k = a_k * p_nm1 + p_nm2;

        /* Shift previous numerators for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_k;

        /* Return the requested term */
        if (k == n) {
            return p_k;
        }

        /* Generate the next partial quotient a_{k+1} */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Advance the recurrence variables */
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    /* Unreachable, but keeps the compiler happy */
    return 0;
}

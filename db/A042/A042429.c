/*
 * A042429  Denominators of continued fraction convergents to sqrt(742).
 * https://oeis.org/A042429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(742). Let b0 =
 *            floor(sqrt(742)) = 27. For continued‑fraction terms a_k (k ≥ 0):
 *            m_0 = 0, d_0 = 1, a_0 = b0 m_{k+1} = d_k * a_k - m_k d_{k+1} =
 *            (742 - m_{k+1}^2) / d_k a_{k+1} = (b0 + m_{k+1}) / d_{k+1}
 *            (integer division) Convergents (p_k , q_k): p...
 * Range:     u8 none, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Integer part of sqrt(742). */
    const seq_t D   = (seq_t)742;
    const seq_t a0  = (seq_t)floor_sqrt(D);

    /* Continued‑fraction parameters. */
    seq_t m     = (seq_t)0;
    seq_t d     = (seq_t)1;
    seq_t a_cur = a0;      /* a_k, starting with a0 */

    /* Convergents: p_k (numerator) and q_k (denominator). */
    seq_t p_prev = (seq_t)1;  /* p_{-1} */
    seq_t p_cur  = a0;       /* p_0 */
    seq_t q_prev = (seq_t)0;  /* q_{-1} */
    seq_t q_cur  = (seq_t)1;  /* q_0 */

    if (n == (idx_t)0)
        return q_cur;      /* a(0) = 1 */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* Next continued‑fraction term. */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Advance parameters. */
        m     = m_next;
        d     = d_next;
        a_cur = a_next;

        /* Update convergents. */
        seq_t p_next = a_cur * p_cur + p_prev;
        seq_t q_next = a_cur * q_cur + q_prev;

        /* Shift for next iteration. */
        p_prev = p_cur;
        p_cur  = p_next;
        q_prev = q_cur;
        q_cur  = q_next;
    }

    return q_cur;  /* q_n is the required denominator. */
}

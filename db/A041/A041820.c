/*
 * A041820  Numerators of continued fraction convergents to sqrt(431).
 * https://oeis.org/A041820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = numerator of the n‑th convergent of the continued fraction
 *        for sqrt(431).  The continued fraction is generated
 *        iteratively:  m_{k+1} = d_k*a_k - m_k,
 *        d_{k+1} = (431 - m_{k+1}^2)/d_k,
 *        a_{k+1} = floor((20 + m_{k+1})/d_{k+1}).
 *        The recurrence for the numerators is
 *          p_{-2} = 0,  p_{-1} = 1,
 *          p_k   = a_k * p_{k-1} + p_{k-2}.
 *        a_0 is floor(sqrt(431)) = 20.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(431) */
    const idx_t D   = 431;
    const idx_t a0  = 20;          /* floor(sqrt(431)) */

    if (n == 0)
        return (seq_t)a0;         /* p_0 = a0 */

    /* Continued fraction state */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Convergent recurrence state */
    seq_t p_nm2 = 0;              /* p_{-2} */
    seq_t p_nm1 = 1;              /* p_{-1} */
    seq_t p_n   = 0;

    /* First convergent (k = 0) */
    p_n = (seq_t)a * p_nm1 + p_nm2;        /* p_0 = a0 * 1 + 0 */
    if (n == 0)
        return p_n;
    p_nm2 = p_nm1;                    /* p_{-1} -> p_{-2} */
    p_nm1 = p_n;                      /* p_0    -> p_{-1} */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Generate next partial quotient */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        /* Compute next numerator */
        p_n = (seq_t)a * p_nm1 + p_nm2;

        if (k == n)
            return p_n;

        /* Shift for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    /* Unreachable but keeps compiler happy */
    return (seq_t)0;
}

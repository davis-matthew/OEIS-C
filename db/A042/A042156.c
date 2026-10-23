/*
 * A042156  Numerators of continued fraction convergents to sqrt(603).
 * https://oeis.org/A042156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 97684*a(n-10) - a(n-20) for n ≥ 20 Sequence is the
 *            numerators of the convergents to √603. The function computes the
 *            n‑th term directly from the continued‑fraction expansion of √603
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for the quadratic √603 */
    const seq_t A0 = (seq_t)24;   /* floor(√603)          */
    const seq_t D  = (seq_t)603;  /* the discriminant     */

    /* Convergents: p_{-2} = 0, p_{-1} = 1           */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t p       = (seq_t)0;

    /* Continued‑fraction parameters for √603   */
    seq_t m     = (seq_t)0;
    seq_t d     = (seq_t)1;
    seq_t a_cf  = A0;   /* a_0 = floor(√603)       */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        /* Compute the k‑th numerator p_k */
        if (k == (idx_t)0)
            p = a_cf;
        else
            p = a_cf * p_prev1 + p_prev2;

        if (k == n)
            return p;

        /* Advance to the next partial quotient a_{k+1} */
        seq_t m_next = d * a_cf - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        /* Shift the previous convergents */
        p_prev2 = p_prev1;
        p_prev1 = p;

        /* Update continued‑fraction parameters */
        m    = m_next;
        d    = d_next;
        a_cf = a_next;
    }

    /* Unreachable – keep compiler happy */
    return p;
}

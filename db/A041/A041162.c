/*
 * A041162  Numerators of continued fraction convergents to sqrt(91).
 * https://oeis.org/A041162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of the continued
 *            fraction for sqrt(91). The recurrence used: p_(-2)=0, p_(-1)=1
 *            a_0 = floor(sqrt(91)) = 9 For all k ≥ 0 m_(k+1) = d_k * a_k -
 *            m_k d_(k+1) = (91 - m_(k+1)^2) / d_k a_(k+1) = floor((9 +
 *            m_(k+1)) / d_(k+1)) p_k = a_k * p_(k-1) + p_(k-2) The fun...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D = 91ULL;   /* D = 91 */
    const uint64_t a0 = 9ULL;   /* floor(sqrt(91)) */

    seq_t p_prevprev = 0;  /* p_(k-2) */
    seq_t p_prev     = 1;  /* p_(k-1) */
    uint64_t m = 0;        /* m_k */
    uint64_t d = 1;        /* d_k */
    uint64_t a_coeff = a0; /* a_k */

    seq_t new_p = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute p_k */
        new_p = (seq_t)a_coeff * p_prev + p_prevprev;

        if (i == n)
            return new_p;   /* finished */

        /* update CF parameters for next step */
        uint64_t m_new = d * a_coeff - m;
        uint64_t d_new = (D - m_new * m_new) / d;
        uint64_t a_next = (a0 + m_new) / d_new;

        m = m_new;
        d = d_new;
        a_coeff = a_next;

        /* shift previous two numerators */
        p_prevprev = p_prev;
        p_prev = new_p;
    }

    /* unreachable, but keep compiler happy */
    return new_p;
}

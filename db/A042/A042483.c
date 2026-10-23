/*
 * A042483  Denominators of continued fraction convergents to sqrt(769).
 * https://oeis.org/A042483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominators of continued fraction convergents to
 *            sqrt(769). Computed by the standard recurrence for continued
 *            fractions of sqrt(D), with D = 769 and a0 = floor(sqrt(769)) =
 *            27
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Parameters for sqrt(769) */
    const uint64_t D   = 769ULL;
    const uint64_t a0  = 27ULL;   /* floor(sqrt(D)) */

    /* Trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Continued fraction iterators */
    uint64_t m = 0U;
    uint64_t d = 1U;
    uint64_t a_curr = a0;

    /* q_{-2} = 1, q_{-1} = 0  (convergents denominators) */
    seq_t q_prevprev = 1;   /* q_{-2} */
    seq_t q_prev     = 0;   /* q_{-1} */
    seq_t q_curr     = (seq_t)a_curr * q_prev + q_prevprev; /* q0 = 1 */

    /* iterate to find q_n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* next partial quotient a_{i} from previous m,d,a_curr */
        uint64_t m_next = d * a_curr - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* update continued‑fraction state */
        m = m_next;
        d = d_next;
        a_curr = a_next;

        /* convergents recurrence: q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t q_next = (seq_t)a_curr * q_curr + q_prev;

        /* shift previous values */
        q_prevprev = q_prev;
        q_prev     = q_curr;
        q_curr     = q_next;
    }

    return q_curr;
}

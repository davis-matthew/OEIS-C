/*
 * A042937  Denominators of continued fraction convergents to sqrt(1000).
 * https://oeis.org/A042937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 24, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents of sqrt(1000).
   Sequence defined by:
   b_{-1} = 0, b_0 = 1.
   For n ≥ 1:
       b_n = a_n * b_{n-1} + b_{n-2},
   where a_n are the partial quotients of the continued fraction for sqrt(1000)
   (a_0 = 31).  The function computes a_n on the fly. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    /* initial continued fraction data for sqrt(1000) */
    const idx_t N  = 1000u;      /* the number under the square root          */
    const idx_t A0 = 31u;        /* floor(sqrt(1000))                        */
    idx_t m   = 0u;              /* current m                                */
    idx_t d   = 1u;              /* current d                                */
    idx_t a   = A0;              /* current a (partial quotient)             */

    seq_t q_prevprev = 0;        /* b_{-1} */
    seq_t q_prev     = 1;        /* b_0   */

    for (idx_t i = 1u; i <= n; ++i) {
        /* compute next partial quotient a_new */
        idx_t m_new = d * a - m;
        idx_t d_new = (N - m_new * m_new) / d;
        idx_t a_new = (A0 + m_new) / d_new;

        /* compute new denominator b_i */
        seq_t q_new = (seq_t)a_new * q_prev + q_prevprev;

        /* shift the state for the next iteration */
        m = m_new;
        d = d_new;
        a = a_new;
        q_prevprev = q_prev;
        q_prev = q_new;
    }
    return q_prev;
}

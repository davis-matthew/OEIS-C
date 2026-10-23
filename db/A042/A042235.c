/*
 * A042235  Denominators of continued fraction convergents to sqrt(643).
 * https://oeis.org/A042235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent in the continued
 *            fraction expansion of sqrt(643). The recurrence used is the
 *            standard one for continued fractions of quadratic irrationals
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(643) */
    const uint64_t D   = 643ULL;
    const uint64_t A0  = 25ULL;          /* floor(sqrt(643)) */

    /* variables for the continued fraction expansion */
    uint64_t m = 0;                      /* m_0 */
    uint64_t d = 1;                      /* d_0 */
    uint64_t ak = A0;                    /* a_k, starts with a_0 = A0 */

    /* variables for the convergent recurrence */
    seq_t q_prev2 = 1;                   /* q_{-2} */
    seq_t q_prev  = 0;                   /* q_{-1} */
    seq_t q = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* compute q_i */
        q = (seq_t)ak * q_prev + q_prev2;
        if (i == n) {
            return q;
        }

        /* prepare a_{k+1} */
        uint64_t m_next = d * ak - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        ak = (A0 + m_next) / d_next;

        /* shift for next iteration */
        m = m_next;
        d = d_next;
        q_prev2 = q_prev;
        q_prev  = q;
    }

    /* unreachable, but keeps the compiler happy */
    return q;
}

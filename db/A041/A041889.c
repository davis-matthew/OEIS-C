/*
 * A041889  Denominators of continued fraction convergents to sqrt(466).
 * https://oeis.org/A041889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of sqrt(466) OEIS:
 *            A041889 (Continued fraction convergents to sqrt(466))
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(466) continued fraction */
    const uint64_t D = 466ULL;          /* number under the square root */
    const uint64_t A0 = 21ULL;          /* floor(sqrt(466)) */

    /* Recurrence for convergents:
       p_k = a_k * p_{k-1} + p_{k-2}
       q_k = a_k * q_{k-1} + q_{k-2}
       with initial values p_{-2}=0, p_{-1}=1,
                           q_{-2}=1, q_{-1}=0.
       a_k are the continuing fraction terms of sqrt(466). */
    seq_t p_prev2 = 0, p_prev1 = 1;
    seq_t q_prev2 = 1, q_prev1 = 0;

    uint64_t m = 0, d = 1, a_k = A0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute current denominator q_k */
        seq_t a_seq = (seq_t)a_k;
        seq_t q = a_seq * q_prev1 + q_prev2;

        if (k == n)
            return q;           /* finished; return a(n) */

        /* compute next continued fraction term a_{k+1} */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        a_k = (A0 + m_next) / d_next;
        m = m_next;
        d = d_next;

        /* shift previous values for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = a_seq * p_prev1 + p_prev2;   /* p_k (unused but needed for shift) */

        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    /* Should never reach here */
    return 0;
}

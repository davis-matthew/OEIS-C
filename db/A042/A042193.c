/*
 * A042193  Denominators of continued fraction convergents to sqrt(621).
 * https://oeis.org/A042193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth continued fraction convergent to
 *            sqrt(621) (OEIS A042193). The convergents satisfy p_n = a_n *
 *            p_{n-1} + p_{n-2}, q_n = a_n * q_{n-1} + q_{n-2}, where a_n are
 *            the continued–fraction terms of sqrt(621) with a_0 =
 *            floor(sqrt(621)) = 24. The function returns q_n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(621) */
    const idx_t D   = 621;   /* D = 621 */
    const idx_t a0  = 24;    /* floor(sqrt(621)) */

    /* state variables for the continued fraction */
    idx_t m = 0;          /* m_0 */
    idx_t d = 1;          /* d_0 */
    idx_t a = a0;         /* a_0 */

    /* recurrence variables for convergents */
    seq_t p2 = (seq_t)0;  /* p_-2 */
    seq_t p1 = (seq_t)1;  /* p_-1 */
    seq_t q2 = (seq_t)1;  /* q_-2 */
    seq_t q1 = (seq_t)0;  /* q_-1 */

    seq_t result = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        /* compute current convergent */
        seq_t p = (seq_t)a * p1 + p2;
        seq_t q = (seq_t)a * q1 + q2;

        if (i == n) {
            result = q;
            break;
        }

        /* shift for next iteration */
        p2 = p1;  p1 = p;
        q2 = q1;  q1 = q;

        /* advance continued‑fraction terms for next a */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    return result;
}

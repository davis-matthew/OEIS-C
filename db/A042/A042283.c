/*
 * A042283  Denominators of continued fraction convergents to sqrt(667).
 * https://oeis.org/A042283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued *
 *            fraction of sqrt(667). The recurrence for successive convergents
 *            * (p_k, q_k) is: * p_{-1}=1, p_0=floor(sqrt(667)), q_{-1}=0,
 *            q_0=1 * p_k = a_k p_{k-1} + p_{k-2} * q_k = a_k q_{k-1} +
 *            q_{k-2} * a_k = floor((floor(sqrt(667)) + m_k)/d_k) *...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(667) */
    const idx_t D   = 667;         /* the square‑free part */
    const idx_t a0  = 25;          /* floor(sqrt(667)) */

    /* current continued fraction state */
    idx_t m   = 0;                 /* m_0 */
    idx_t d   = 1;                 /* d_0 */
    idx_t aa  = a0;                /* a_{k-1} (starts as a0) */

    /* convergent denominators: q_{-1}=0, q_0=1 */
    seq_t q_prev = 0;              /* q_{k-2} */
    seq_t q_curr = 1;              /* q_{k-1} */

    if (n == 0) {
        return q_curr;             /* a(0) = 1 */
    }

    for (idx_t k = 1; k <= n; ++k) {
        /* compute next partial quotient a_k */
        idx_t m_next = d * aa - m;                        /* m_k */
        idx_t d_next = (D - m_next * m_next) / d;         /* d_k */
        idx_t a_next = (a0 + m_next) / d_next;            /* a_k */

        /* compute next convergent denominator q_k */
        seq_t q_next = ((seq_t)a_next) * q_curr + q_prev;

        /* shift for next iteration */
        m = m_next;
        d = d_next;
        aa = a_next;
        q_prev = q_curr;
        q_curr = q_next;
    }

    return q_curr;
}

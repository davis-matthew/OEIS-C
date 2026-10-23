/*
 * A042769  Denominators of continued fraction convergents to sqrt(915).
 * https://oeis.org/A042769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(915) */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t N = 915ULL;          /* irrational part of the quadratic surd */
    /* floor(sqrt(915)) is 30 */
    const uint64_t a0 = 30ULL;

    /* n == 0 corresponds to the first convergent 30/1, denominator 1 */
    if (n == 0) return (seq_t)1;

    /* Variables for the standard continued‑fraction algorithm */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_curr = a0;

    /* Convergents: p_{-1}=1, p_0=a0; q_{-1}=0, q_0=1 */
    seq_t p_prev = (seq_t)1;
    seq_t p_curr = (seq_t)a_curr;
    seq_t q_prev = (seq_t)0;
    seq_t q_curr = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* next partial quotient */
        uint64_t m_next = d * a_curr - m;
        uint64_t d_next = (N - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* update convergents: p_n = a_n p_{n-1} + p_{n-2} */
        seq_t p_next = ((seq_t)a_next) * p_curr + p_prev;
        seq_t q_next = ((seq_t)a_next) * q_curr + q_prev;

        /* shift for next iteration */
        p_prev = p_curr;   p_curr = p_next;
        q_prev = q_curr;   q_curr = q_next;
        m = m_next;        d = d_next;
        a_curr = a_next;
    }

    return q_curr;   /* denominator of the nth convergent */
}

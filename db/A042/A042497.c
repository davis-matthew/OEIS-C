/*
 * A042497  Denominators of continued fraction convergents to sqrt(776).
 * https://oeis.org/A042497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042497: denominators of continued fraction convergents to sqrt(776) */
#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long N  = 776ULL;  /* the squarefree part of 776 */
    const unsigned long long A0 = 27ULL;   /* floor(sqrt(776)) */

    /* a(0) is the first denominator (q0 = 1) */
    if (n == 0) return (seq_t)1;

    /* Continued‑fraction state */
    unsigned long long m = 0;   /* m_k */
    unsigned long long d = 1;   /* d_k */
    unsigned long long a = A0;  /* a_k */

    /* Convergent recurrence: p_{-1}=1, p_0=A0;  q_{-1}=0, q_0=1 */
    seq_t p_minus1 = (seq_t)1;
    seq_t p_prev   = (seq_t)A0;
    seq_t q_minus1 = (seq_t)0;
    seq_t q_prev   = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* next continued‑fraction coefficients */
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (N - m_next * m_next) / d;
        unsigned long long a_next = (A0 + m_next) / d_next;

        /* compute next convergents */
        seq_t p_cur = (seq_t)a_next * p_prev + p_minus1;
        seq_t q_cur = (seq_t)a_next * q_prev + q_minus1;

        /* shift variables for the next iteration */
        p_minus1 = p_prev;  p_prev = p_cur;
        q_minus1 = q_prev;  q_prev = q_cur;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    return q_prev;   /* q_n is the desired denominator */
}

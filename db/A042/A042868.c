/*
 * A042868  Numerators of continued fraction convergents to sqrt(966).
 * https://oeis.org/A042868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction convergent numerators for sqrt(966).
 * p_{-2} = 0, p_{-1} = 1
 * p_n   = a_n * p_{n-1} + p_{n-2}
 * where a_0 = 31 and for k ≥ 0:
 *   m_{k+1} = d_k * a_k - m_k
 *   d_{k+1} = (966 - m_{k+1}^2) / d_k
 *   a_{k+1} = (31 + m_{k+1}) / d_{k+1}
 * (integer division).  This yields a(n) = numerator of the
 * nth convergent of √966. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* quick path for the first term */
    if (n == 0)
        return (seq_t)31;

    const uint64_t D    = 966ULL;   /* D in √D */
    const uint64_t A0   = 31ULL;    /* floor(√D) */

    uint64_t m = 0;          /* m_0 */
    uint64_t d = 1;          /* d_0 */
    seq_t   a_cur = (seq_t)A0;    /* a_0 */

    seq_t p_prev2 = (seq_t)0;       /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;       /* p_{-1} */
    seq_t p_cur;                    /* will hold p_n */

    /* generate a_1 ... a_{n-1} and the corresponding p_{0} ... p_{n-1} */
    for (idx_t i = 0; i < n; ++i) {
        p_cur = a_cur * p_prev1 + p_prev2;

        /* shift previous two convergents */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;

        /* compute next continued‑fraction coefficient a_{i+1} */
        uint64_t m_next = d * (uint64_t)a_cur - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (A0 + m_next) / d_next;

        m      = m_next;
        d      = d_next;
        a_cur  = (seq_t)a_next;
    }

    /* compute the nth convergent using the last a_cur (= a_n) */
    p_cur = a_cur * p_prev1 + p_prev2;
    return p_cur;
}

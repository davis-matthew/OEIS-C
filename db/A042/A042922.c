/*
 * A042922  Numerators of continued fraction convergents to sqrt(993).
 * https://oeis.org/A042922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the continued
 *            fraction of sqrt(993). Recurrence: let a0 = floor(sqrt(993)) =
 *            31. define m0 = 0, d0 = 1, for k >= 0: m_{k+1} = d_k * a_k - m_k
 *            d_{k+1} = (993 - m_{k+1}^2) / d_k a_{k+1} = floor((a0 + m_{k+1})
 *            / d_{k+1}) numerators: p_{-1} = 1, p_{-2} = 0,...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D   = 993;
    const uint64_t a0  = 31;          /* floor(sqrt(D)) */

    /* n == 0 is specially handled: the first numerator is a0 */
    if (n == 0)
        return (seq_t)a0;

    /* recurrence setup */
    seq_t p_nm2 = 0;          /* p_{-2} */
    seq_t p_nm1 = 1;          /* p_{-1} */
    seq_t p_nm0 = (seq_t)a0;  /* p_0 */

    uint64_t m   = 0;
    uint64_t d   = 1;
    uint64_t a_k = a0;

    /* iterate from k = 1 to n, computing p_k each time */
    for (idx_t i = 1; i <= n; ++i) {
        /* next partial quotient a_{k} */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* update state for next iteration */
        m = m_next;
        d = d_next;
        a_k = a_next;

        /* compute next numerator p_k */
        seq_t p_next = (seq_t)a_k * p_nm0 + p_nm1;

        /* shift for next step */
        p_nm1 = p_nm0;
        p_nm0 = p_next;
    }

    return p_nm0;   /* p_n */
}

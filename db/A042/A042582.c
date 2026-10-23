/*
 * A042582  Numerators of continued fraction convergents to sqrt(820).
 * https://oeis.org/A042582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(820) *
 *            Recurrence: p_{-1}=1, p_0 = floor(sqrt(820)) = 28 * For k>=1:
 *            p_k = a_k p_{k-1} + p_{k-2}, where a_k are the * continued
 *            fraction partial quotients of sqrt(820)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(820) */
    const uint64_t D  = 820ULL;
    const uint64_t a0 = 28ULL;

    /* Base convergent values */
    seq_t p_nm2 = 1;              /* p_{-1} */
    seq_t p_nm1 = (seq_t)a0;      /* p_0 */

    if (n == 0) {
        return p_nm1;
    }

    uint64_t m     = 0;           /* m_0 */
    uint64_t d     = 1;           /* d_0 */
    uint64_t a_prev = a0;         /* a_{-1} = a_0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Next m, d, and a_k following the standard algorithm */
        uint64_t m_next = d * a_prev - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_k    = (a0 + m_next) / d_next;

        /* Compute the new numerator */
        seq_t p_k = (seq_t)a_k * p_nm1 + p_nm2;

        /* Shift variables for next iteration */
        m = m_next;
        d = d_next;
        a_prev = a_k;
        p_nm2 = p_nm1;
        p_nm1 = p_k;
    }

    return p_nm1;
}

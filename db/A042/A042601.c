/*
 * A042601  Denominators of continued fraction convergents to sqrt(829).
 * https://oeis.org/A042601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominator of the nth convergent of the continued fraction of
 *            sqrt(829). Recurrence: p_-1 = 1, p_0 = floor(sqrt(829)) = 28
 *            q_-1 = 0, q_0 = 1 for k >= 1: a_k = periodic part of the
 *            continued fraction (length 17) p_k = a_k * p_{k-1} + p_{k-2} q_k
 *            = a_k * q_{k-1} + q_{k-2} a(0) = q_0 = 1, a(n) = q_n
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic coefficients of continued fraction of sqrt(829) after a0=28 */
    static const uint16_t period_terms[17] = {
        1, 3, 1, 4, 2, 3, 2, 1, 1, 2, 3, 2, 4, 1, 3, 1, 56
    };
    const uint16_t period = 17;     /* length of the periodic part */
    if (n == 0) return (seq_t)1;    /* q_0 = 1 */

    seq_t q_prevprev = (seq_t)0;    /* q_{-1} */
    seq_t q_prev     = (seq_t)1;    /* q_0 */

    for (idx_t k = 1; k <= n; ++k) {
        uint16_t a_k = period_terms[(k - 1) % period];
        seq_t q_k = (seq_t)a_k * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_k;
    }
    return q_prev;
}

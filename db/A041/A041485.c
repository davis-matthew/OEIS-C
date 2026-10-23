/*
 * A041485  Denominators of continued fraction convergents to sqrt(259).
 * https://oeis.org/A041485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: denominators of continued‑fraction convergents to sqrt(259)
 * Recurrence (generated from the continued fraction of sqrt(259)):
 *    a(0) = 1
 *    for n >= 1:
 *       let a0 = floor(sqrt(259)) = 16
 *       maintain (m, d, a) with:
 *           m_0 = 0, d_0 = 1, a_0 = 16
 *           m_{k+1} = d_k * a_k - m_k
 *           d_{k+1} = (259 - m_{k+1}^2) / d_k
 *           a_{k+1} = (a0 + m_{k+1}) / d_{k+1}
 *       and q_{-1} = 0, q_0 = 1
 *           q_k = a_k * q_{k-1} + q_{k-2}
 *       then a(n) = q_n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(259) */
    const idx_t D  = (idx_t)259;
    const idx_t a0 = (idx_t)16;   /* floor(sqrt(259)) */

    /* continued fraction state */
    idx_t m = 0;   /* m_0 */
    idx_t d = 1;   /* d_0 */
    idx_t a_cf = a0;   /* a_0 */

    /* convergent numerators/denominators (track only denominators) */
    seq_t q_m2 = 0;  /* q_{-1} */
    seq_t q_m1 = 1;  /* q_0 (a(0)) */
    seq_t q = 0;     /* placeholder for current denominator */

    if (n == 0)
        return q_m1;   /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* update continued‑fraction parameters */
        m = d * a_cf - m;
        d = (D - m * m) / d;
        a_cf = (a0 + m) / d;

        /* compute next denominator */
        q = (seq_t)a_cf * q_m1 + q_m2;

        /* shift for next iteration */
        q_m2 = q_m1;
        q_m1 = q;
    }

    return q_m1;   /* a(n) */
}

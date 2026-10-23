/*
 * A042179  Denominators of continued fraction convergents to sqrt(614).
 * https://oeis.org/A042179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to √614 Recurrence
 *            uses the standard continued‑fraction recurrence: q_{-2} = 1,
 *            q_{-1} = 0 q_k = a_k q_{k-1} + q_{k-2} with a_0 = ⌊√614⌋ = 24
 *            and the subsequent a_k derived by the periodic
 *            continued–fraction algorithm
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* floor(sqrt(614)) */
    const idx_t a0 = 24;
    /* constant D = 614 for the continued fraction of sqrt(614) */
    const idx_t D  = 614;

    /* Variables for the periodic continued‑fraction algorithm */
    idx_t m = 0;            /* m_0 */
    idx_t d = 1;            /* d_0 */
    idx_t a = a0;           /* current partial quotient */

    /* Convergent denominators: q_{-2} = 1, q_{-1} = 0 */
    seq_t q_m2 = 1;         /* q_{k-2} */
    seq_t q_m1 = 0;         /* q_{k-1} */
    seq_t q = 0;            /* q_k */

    for (idx_t i = 0; i <= n; ++i) {
        if (i > 0) { /* compute next partial quotient */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;        /* floor division */
        }
        /* compute denominator of the k-th convergent */
        q = (seq_t)a * q_m1 + q_m2;
        /* shift for next iteration */
        q_m2 = q_m1;
        q_m1 = q;
    }

    return q;               /* q_n is the required term a(n) */
}

/*
 * A041803  Denominators of continued fraction convergents to sqrt(422).
 * https://oeis.org/A041803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of sqrt(422)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 422 is the number under the square root.
       floor(sqrt(422)) = 20. */
    const idx_t D = 422;
    const idx_t a0 = 20;          /* floor(sqrt(D)) */

    /* Continued–fraction parameters for sqrt(D) */
    idx_t m = 0;                  /* m_0             */
    idx_t d = 1;                  /* d_0             */
    idx_t a_k = a0;               /* a_0             */

    /* Denominators of convergents: Q_{-2} = 1, Q_{-1} = 0 */
    seq_t q_prevprev = 1;
    seq_t q_prev     = 0;
    seq_t q_curr     = 0;

    for (idx_t i = 0; i <= n; ++i) {
        q_curr = (seq_t)a_k * q_prev + q_prevprev;
        if (i == n)
            return q_curr;

        /* Shift for the next iteration */
        q_prevprev = q_prev;
        q_prev = q_curr;

        /* Update continued‑fraction parameters */
        m = d * a_k - m;
        d = (D - m * m) / d;
        a_k = (a0 + m) / d;
    }

    /* Unreachable, but satisfies compiler */
    return 0;
}

/*
 * A041802  Numerators of continued fraction convergents to sqrt(422).
 * https://oeis.org/A041802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of continued fraction convergents to sqrt(422)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const idx_t D         = 422;   /* discriminant */
    const idx_t SQRT_D    = 20;    /* floor(sqrt(422)) */

    idx_t m   = 0;          /* m(0) */
    idx_t d   = 1;          /* d(0) */
    idx_t a_i = SQRT_D;     /* a(0) */

    seq_t p_prevprev = 0;   /* p(-2) */
    seq_t p_prev     = 1;   /* p(-1) */

    for (idx_t i = 0; i <= n; ++i) {
        /* compute p_i = a_i * p_prev + p_prevprev */
        seq_t p = (seq_t)a_i * p_prev + p_prevprev;

        if (i == n) {
            return p;       /* the requested numerator */
        }

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = p;

        /* compute next partial quotient a_{i+1} */
        idx_t m_next = d * a_i - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (SQRT_D + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_i = a_next;
    }

    return 0;  /* unreachable */
}

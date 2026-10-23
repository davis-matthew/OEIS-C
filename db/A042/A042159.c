/*
 * A042159  Denominators of continued fraction convergents to sqrt(604).
 * https://oeis.org/A042159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Denominators of the convergents to √604. Computed by the
 *            standard continued‑fraction recurrence: q_{-2}=1, q_{-1}=0, q_n
 *            = a_n * q_{n-1} + q_{n-2}, where a_n are the partial quotients
 *            of √604
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build system. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* The first two partial quotients of √604 are 24 and 1. */
    const idx_t D = 604u;      /* the square‑free part in the Radical          */
    const idx_t a0 = 24u;     /* floor(√604)                                 */

    /* q_{-2}=1, q_{-1}=0 */
    seq_t q_prevprev = 1;
    seq_t q_prev     = 0;

    /* Current continued‑fraction variables */
    idx_t m = 0, d = 1, a_coeff = a0;

    /* Generate partial quotients and convergents on the fly. */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t q_cur = (seq_t)a_coeff * q_prev + q_prevprev;
        if (i == n)
            return q_cur;          /* q_n found */

        /* shift for next iteration */
        q_prevprev = q_prev;
        q_prev = q_cur;

        /* next elements of the continued fraction */
        idx_t m_next = d * a_coeff - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_coeff = a_next;
    }
    /* This point is unreachable for non‑negative n. */
    return 0;
}

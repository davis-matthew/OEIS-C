/*
 * A041504  Numerators of continued fraction convergents to sqrt(269).
 * https://oeis.org/A041504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of convergents to sqrt(269), a(0)=16, a(1)=33,
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for idx_t, seq_t */

seq_t a(idx_t n) {
    /* sqrt(269) continued fraction a0 = floor sqrt. */
    const idx_t D = 269;
    const idx_t a0 = 16;   /* floor(sqrt(269)) */

    if (n == 0)
        return (seq_t)a0;

    /* Convergents recurrence: p_-1 = 1, p_0 = a0 */
    seq_t p_prevprev = 1;               /* p_-1 */
    seq_t p_prev = (seq_t)a0;           /* p_0 */

    /* Continued fraction parameters */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_k = a0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next partial quotient a_k */
        idx_t m_new = d * a_k - m;
        idx_t d_new = (D - m_new * m_new) / d;
        idx_t a_new = (a0 + m_new) / d_new;

        /* Update parameters */
        m = m_new;
        d = d_new;
        a_k = a_new;

        /* p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p = (seq_t)a_k * p_prev + p_prevprev;

        /* Shift for next iteration */
        p_prevprev = p_prev;
        p_prev = p;
    }

    return p_prev;   /* p_n for requested n */
}

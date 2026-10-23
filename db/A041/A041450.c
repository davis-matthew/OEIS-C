/*
 * A041450  Numerators of continued fraction convergents to sqrt(241).
 * https://oeis.org/A041450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 15, a(1) = 16, and for n ≥ 2 a(n) = a_n * a(n-1) + a(n-2)
 *            where a_n are the partial quotients in the continued fraction of
 *            sqrt(241)
 * Range:     u8 none, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    const idx_t D = 241;
    seq_t sqrtD = floor_sqrt((seq_t)D);
    idx_t a0 = (idx_t)sqrtD;          // integer part of sqrt(241)

    if (n == 0) return sqrtD;        // a(0)

    seq_t num_prevprev = (seq_t)1;    // p_{-1}
    seq_t num_prev = sqrtD;           // p_0

    idx_t m = 0, d = 1, a_part = a0; // state for continued fraction

    for (idx_t k = 1; k <= n; ++k) {
        /* compute next partial quotient a_{k} */
        idx_t m_next = d * a_part - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* convergent numerator */
        seq_t pk = (seq_t)a_next * num_prev + num_prevprev;

        /* shift for next iteration */
        num_prevprev = num_prev;
        num_prev = pk;

        /* update continued fraction state */
        m = m_next;
        d = d_next;
        a_part = a_next;
    }

    return num_prev;
}

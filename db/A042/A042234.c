/*
 * A042234  Numerators of continued fraction convergents to sqrt(643).
 * https://oeis.org/A042234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042234: a(n) = numerator of the nth convergent to √643 (offset 0) */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Periodic continued fraction of √643 */
    const idx_t D = 643;          /* D = 643 */
    const idx_t a0 = 25;          /* floor(√643) = 25 */

    /* m_k, d_k, a_k for the continued fraction */
    idx_t m = 0;  /* m_0 */
    idx_t d = 1;  /* d_0 */
    idx_t a = a0; /* a_0 */

    /* Convergent numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_prev2 = 0;  /* p_{-2} */
    seq_t p_prev1 = 1;  /* p_{-1} */
    seq_t p_curr;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute p_k = a_k * p_{k-1} + p_{k-2} */
        p_curr = ((seq_t)a) * p_prev1 + p_prev2;
        if (k == n) {
            return p_curr;   /* desired numerator */
        }

        /* Prepare next a_{k+1} */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Shift for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    /* Should never reach here */
    return 0;
}

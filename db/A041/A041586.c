/*
 * A041586  Numerators of continued fraction convergents to sqrt(311).
 * https://oeis.org/A041586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(311) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Standard continued‑fraction recurrence for sqrt(D) */
    const uint64_t D  = 311;         /* square‑free part */
    const uint64_t a0 = 17;          /* floor(sqrt(311)) */

    /* p[-2] = 0, p[-1] = 1  (C–F numerators) */
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;

    /* Continued‑fraction state variables */
    uint64_t m = 0;      /* m_0 */
    uint64_t d = 1;      /* d_0 */
    uint64_t a_k = a0;   /* a_0 */

    for (idx_t i = 0; ; ++i) {
        /* Compute the i‑th numerator. */
        seq_t p_curr = (seq_t)a_k * p_minus1 + p_minus2;

        /* Return if this is the desired term. */
        if (i == n) return p_curr;

        /* Update for the next partial quotient. */
        uint64_t m_next = a_k * d - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Shift state for next iteration. */
        m = m_next;
        d = d_next;
        a_k = a_next;
        p_minus2 = p_minus1;
        p_minus1 = p_curr;
    }
}

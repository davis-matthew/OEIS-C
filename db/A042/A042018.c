/*
 * A042018  Numerators of continued fraction convergents to sqrt(533).
 * https://oeis.org/A042018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   Numerators of continued fraction convergents to sqrt(533)
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 23, a(1) = 254, ...  (OEIS A042018) */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Constant for the square root of 533 */
    const idx_t D = 533;
    /* integer part of sqrt(D) */
    const idx_t a0 = (idx_t)floor_sqrt((seq_t)D);

    /* Continued fraction state variables */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_k = a0;

    /* Recurrence variables for numerators */
    seq_t p_prev2 = 0;  /* p_{-2} */
    seq_t p_prev1 = 1;  /* p_{-1} */
    seq_t p_curr;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute current numerator: p_k = a_k * p_{k-1} + p_{k-2} */
        p_curr = (seq_t)a_k * p_prev1 + p_prev2;
        if (k == n)
            return p_curr;

        /* Shift recurrence variables for next step */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;

        /* Update continued fraction terms for next step */
        idx_t m_new = d * a_k - m;
        idx_t d_new = (D - m_new * m_new) / d;
        idx_t a_next = (a0 + m_new) / d_new;

        m = m_new;
        d = d_new;
        a_k = a_next;
    }
    /* This point is never reached; the loop always returns */
    return 0;
}

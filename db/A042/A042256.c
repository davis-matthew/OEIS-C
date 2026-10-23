/*
 * A042256  Numerators of continued fraction convergents to sqrt(654).
 * https://oeis.org/A042256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the continued
 *            fraction of sqrt(654)
 * Range:     u8 none, u16 n <= 9, u32 n <= 20, u64 n <= 39, u128 n <= 84,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: use the standard recurrence for convergents
   of a quadratic irrational. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D = 654;
    seq_t a0 = floor_sqrt(D);       /* a0 = floor(sqrt(D)) = 25 */

    seq_t m   = 0;                  /* m_0  */
    seq_t d   = 1;                  /* d_0  */
    seq_t a_coeff = a0;             /* a_0  */

    seq_t p_m2 = 0;                 /* p_{-2} */
    seq_t p_m1 = 1;                 /* p_{-1} */
    seq_t p_cur;

    for (idx_t i = 0; i <= n; ++i) {
        /* p_i = a_i * p_{i-1} + p_{i-2} */
        p_cur = a_coeff * p_m1 + p_m2;

        if (i == n)
            return p_cur;

        /* Compute the next continued‑fraction coefficient */
        seq_t m_next  = d * a_coeff - m;
        seq_t d_next  = (D - m_next * m_next) / d;
        seq_t a_next  = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_coeff = a_next;

        /* shift the previous two numerators */
        p_m2 = p_m1;
        p_m1 = p_cur;
    }

    return 0; /* should never be reached */
}

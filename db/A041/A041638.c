/*
 * A041638  Numerators of continued fraction convergents to sqrt(338).
 * https://oeis.org/A041638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041638: Numerators of continued fraction convergents to sqrt(338)
   Recurrence:
       p_{-2} = 0,   p_{-1} = 1
       p_n   = a_n * p_{n-1} + p_{n-2}
   where a_n are the periodic terms of the continued fraction of sqrt(338),
   starting with a_0 = floor(sqrt(338)) = 18. The terms a_n are generated
   by the standard algorithm for quadratic surds.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(338) */
    const seq_t D   = (seq_t)338; /* the radicand */
    const seq_t a0  = (seq_t)18;  /* floor(sqrt(338)) */

    /* Convergent numerators */
    seq_t p_nm2 = (seq_t)0;  /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;  /* p_{-1} */
    seq_t p_n   = (seq_t)0;  /* current p_n */

    /* Continued fraction generators */
    seq_t m = (seq_t)0;      /* m_0 */
    seq_t d = (seq_t)1;      /* d_0 */
    seq_t a_term = a0;       /* a_0 */

    for (idx_t i = (idx_t)0; i <= n; ++i)
    {
        /* Compute convergent numerator p_i */
        p_n = a_term * p_nm1 + p_nm2;

        /* If required index reached, return result */
        if (i == n) return p_n;

        /* Shift for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_n;

        /* Compute next continued fraction term */
        seq_t m_next = d * a_term - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Update generators */
        m      = m_next;
        d      = d_next;
        a_term = a_next;
    }

    /* Unreachable: only to avoid compiler warning */
    return (seq_t)0;
}

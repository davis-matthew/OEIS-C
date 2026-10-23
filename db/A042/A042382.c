/*
 * A042382  Numerators of continued fraction convergents to sqrt(718).
 * https://oeis.org/A042382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(718)
   a(0) = 26, a(1) = 27, a(2) = 107, a(3) = 134, ...
   a(n) is the numerator of the n-th convergent of sqrt(718).  */
#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t D   = 718;           /* D for sqrt(D) */
    const idx_t a0  = 26;            /* floor(sqrt(718)) */

    /* Trivial case */
    if (n == 0)
        return (seq_t)a0;

    /* Continued‑fraction state */
    idx_t m = 0;                     /* m_0  */
    idx_t d = 1;                     /* d_0  */
    idx_t a_cur = a0;                /* a_0  */

    /* Convergent numerators: p_{-1}=1, p_0=a0 */
    seq_t p_prevprev = 1;            /* p_{n-1}   (p_{-1})   */
    seq_t p_prev     = (seq_t)a0;    /* p_n       (p_0 )    */

    seq_t p_curr = 0;                /* temporary storage for p_n+1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next partial quotient a_n */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Recurrence for numerators: p_n = a_n * p_{n-1} + p_{n-2} */
        p_curr = (seq_t)a_next * p_prev + p_prevprev;

        /* Shift variables for next iteration */
        p_prevprev = p_prev;
        p_prev = p_curr;

        /* Update continued‑fraction state */
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    /* After the loop, p_prev holds p_n */
    return p_prev;
}

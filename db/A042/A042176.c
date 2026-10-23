/*
 * A042176  Numerators of continued fraction convergents to sqrt(613).
 * https://oeis.org/A042176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent of the continued
 *            fraction of sqrt(613)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the numerator of the n-th convergent of the simple continued fraction
 * expansion of sqrt(613). The first few values are: 24, 25, 99, 619, 718, …
 * The function computes the value for any n >= 0. */

#include <stdint.h>  /* for uint64_t; seq_t/idx_t are supplied by the build */

seq_t a(idx_t n)
{
    /* Constant sqrt(613) floor */
    static const idx_t D    = 613;
    static const idx_t a0   = 24;   /* floor(sqrt(613)) */

    if (n == 0u)
        return (seq_t)a0;          /* a(0) = 24 */

    /* Standard recurrence for convergents */
    seq_t p_prev = (seq_t)1;        /* p_{-1} */
    seq_t p_curr = (seq_t)a0;       /* p_0  */

    /* Continued fraction generation variables */
    idx_t m = 0;                    /* m_0   */
    idx_t d = 1;                    /* d_0   */
    idx_t a = a0;                   /* a_0   */

    /* Compute successive partial quotients and numerators up to n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Next partial quotient a_k using the standard algorithm for sqrt(D) */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Next numerator p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p_next = (seq_t)a_next * p_curr + p_prev;

        /* Advance state */
        p_prev = p_curr;
        p_curr = p_next;
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return p_curr;
}

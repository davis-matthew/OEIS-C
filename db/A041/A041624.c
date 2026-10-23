/*
 * A041624  Numerators of continued fraction convergents to sqrt(331).
 * https://oeis.org/A041624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            expansion of sqrt(331)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 37, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant data for sqrt(331) */
    const idx_t D  = 331;   /* non‑perfect square */
    const idx_t a0 = 18;    /* floor(sqrt(331)) */

    /* Numerator recurrence: p_{-2} = 0, p_{-1} = 1,
       p_k = a_k * p_{k-1} + p_{k-2}                     */
    seq_t p_prev2 = (seq_t)0;    /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;    /* p_{-1} */
    seq_t result = (seq_t)0;

    /* Continued fraction state for sqrt(D)   */
    idx_t m = (idx_t)0;
    idx_t d = (idx_t)1;
    idx_t a = a0;   /* a_0 */

    /* Handle the trivial case n == 0 directly */
    if (n == (idx_t)0)
        return (seq_t)a0;

    /* Generate convergents up to the requested index */
    for (idx_t i = (idx_t)0; i <= n; ++i) {
        /* Compute p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t p = (seq_t)a * p_prev1 + p_prev2;
        if (i == n)
            result = p;

        /* Rotate previous values for next step */
        p_prev2 = p_prev1;
        p_prev1 = p;

        /* Prepare next a_{i+1} only if we are not finished */
        if (i < n) {
            /* m_{k+1} = d_k * a_k - m_k                    */
            idx_t m_next = d * a - m;

            /* d_{k+1} = (D - m_{k+1}^2) / d_k             */
            idx_t d_next = (D - m_next * m_next) / d;

            /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})  */
            idx_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a = a_next;
        }
    }

    return result;
}

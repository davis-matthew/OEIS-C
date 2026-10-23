/*
 * A042676  Numerators of continued fraction convergents to sqrt(868).
 * https://oeis.org/A042676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Compute the numerators of the convergents of the
 * continued fraction for sqrt(868).
 * The sequence is defined by the recurrence
 *   a(0) = 29
 *   a(n) = a_k * a(n-1) + a(n-2)   for n ≥ 1
 * where a_k is the k-th coefficient of the continued fraction of
 * sqrt(868) (k starts at 1).  The coefficients are generated
 * iteratively using the standard algorithm for quadratic irrationals.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* D = 868, a0 = floor(sqrt(868)) = 29 */
    const idx_t D   = 868;
    const idx_t a0  = 29;

    if (n == 0U) {
        return (seq_t)29U;
    }

    /* previous two numerators: p_{-1} = 1, p_0 = a0 */
    seq_t p_prev2 = (seq_t)1U;
    seq_t p_prev1 = (seq_t)a0;
    seq_t p_curr  = 0U;

    /* continued fraction state: m_0 = 0, d_0 = 1, a_0 = 29 */
    idx_t m = 0U;
    idx_t d = 1U;
    idx_t a = a0;

    for (idx_t i = 1U; i <= n; ++i) {
        /* advance continued fraction coefficients */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* recurrence for numerators */
        p_curr = (seq_t)a_next * p_prev1 + p_prev2;

        /* shift for next step */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return p_curr;
}

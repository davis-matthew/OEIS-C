/*
 * A042250  Numerators of continued fraction convergents to sqrt(651).
 * https://oeis.org/A042250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the nth convergent of the continued
 *            fraction expansion of sqrt(651). Recurrence: a(-1) = 1 , a(0) =
 *            floor(sqrt(651)) a(k) = A(k) * a(k-1) + a(k-2), where A(k) are
 *            the continued‑fraction coefficients of sqrt(651), starting with
 *            A(0)=floor(sqrt(651)). The coefficients are g...
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D = (seq_t)651;            /* D = 651 */
    const seq_t a0 = floor_sqrt(D);       /* integer part of sqrt(651) */

    /* Handle the trivial case n == 0 */
    if (n == 0)
        return a0;

    /* p[-1] and p[0] for the recurrence */
    seq_t p_prev = (seq_t)1;              /* p(-1) = 1 */
    seq_t p_curr = a0;                    /* p(0)  = a0  */

    /* Continued fraction state for sqrt(651) */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_coeff = a0;                   /* A(0)  */

    /* Compute terms up to index n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Generate next coefficient A(i) */
        seq_t m_next = a_coeff * d - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Recurrence for the numerator */
        seq_t p_next = a_next * p_curr + p_prev;

        /* Shift for next iteration */
        p_prev  = p_curr;
        p_curr  = p_next;
        m       = m_next;
        d       = d_next;
        a_coeff = a_next;
    }

    return p_curr;
}

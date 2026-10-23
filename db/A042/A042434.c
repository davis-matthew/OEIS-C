/*
 * A042434  Numerators of continued fraction convergents to sqrt(745).
 * https://oeis.org/A042434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(745) for n >= 0
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Continued fraction parameters for sqrt(D) where D = 745 */
    const idx_t D = 745;
    idx_t a0 = 27;           /* floor(sqrt(745)) */
    idx_t m = 0;             /* intermediate value m_k */
    idx_t d = 1;             /* intermediate value d_k */
    idx_t a = a0;            /* current partial quotient a_k */

    seq_t p2 = 0;            /* p_{k-2} */
    seq_t p1 = 1;            /* p_{k-1} */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p = (seq_t)a * p1 + p2;

        if (k == n)
            return p;       /* desired numerator */

        /* Prepare next partial quotient a_{k+1} */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - (uint64_t)m_next * (uint64_t)m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Shift for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;

        p2 = p1;
        p1 = p;
    }

    /* Should never reach here */
    return 0;
}

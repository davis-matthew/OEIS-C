/*
 * A041148  Numerators of continued fraction convergents to sqrt(84).
 * https://oeis.org/A041148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of continued‑fraction convergents to sqrt(84)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(84) */
    const seq_t D = (seq_t)84;     /* D in continued‑fraction algorithm */
    const seq_t a0 = (seq_t)9;     /* floor(sqrt(84)) */

    /* Variables for continued‑fraction algorithm */
    seq_t m = 0, d = 1, a = a0;
    seq_t m_next, d_next, a_next;

    /* Variables for convergent recurrence p_k = a_k * p_{k-1} + p_{k-2} */
    seq_t prev2 = (seq_t)0;        /* p_{-2} */
    seq_t prev1 = (seq_t)1;        /* p_{-1} */
    seq_t curr;                    /* p_k */

    /* Handle n == 0 directly */
    if (n == 0)
        return a0;

    /* k = 0: p0 = a0 */
    curr = a * prev1 + prev2;      /* a0 * 1 + 0 == a0 */
    prev2 = prev1;                 /* p_{-1} -> prev2 */
    prev1 = curr;                  /* p0 -> prev1 */

    /* Compute successive convergents up to n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute next partial quotient a_{k} */
        m_next = d * a - m;
        d_next = (D - m_next * m_next) / d;
        a_next = (a0 + m_next) / d_next;

        /* Shift for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;

        /* Compute next numerator */
        curr = a * prev1 + prev2;

        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}

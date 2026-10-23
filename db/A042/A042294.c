/*
 * A042294  Numerators of continued fraction convergents to sqrt(673).
 * https://oeis.org/A042294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 21, u64 n <= 39, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(673) computed by a standard continued‑fraction recurrence */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Integer part of sqrt(673) */
    const seq_t A0 = 25;      /* floor(sqrt(673)) */

    if (n == 0)
        return A0;             /* p0 */

    /* Initial conditions for convergent recurrence: p_{-2}=0, p_{-1}=1 */
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;

    /* Compute p0 = a0*p_{-1} + p_{-2} */
    seq_t p0 = A0 * p_minus1 + p_minus2;   /* 25*1 + 0 = 25 */
    p_minus2 = p_minus1;
    p_minus1 = p0;

    /* Continued‑fraction state for sqrt(673) */
    uint64_t m = 0;                     /* m0 */
    uint64_t d = 1;                     /* d0 */
    uint64_t a_k = (uint64_t)A0;        /* a0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Generate next continued‑fraction term a_{k} */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (673ULL - m_next * m_next) / d;
        uint64_t a_next = ((uint64_t)A0 + m_next) / d_next;

        /* Compute the next convergent numerator */
        seq_t a_next_seq = (seq_t)a_next;
        seq_t pk = a_next_seq * p_minus1 + p_minus2;

        /* Shift for next iteration */
        p_minus2 = p_minus1;
        p_minus1 = pk;

        /* Update continued‑fraction state */
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return p_minus1;   /* p_n after loop */
}

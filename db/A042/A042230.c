/*
 * A042230  Numerators of continued fraction convergents to sqrt(641).
 * https://oeis.org/A042230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 18, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(641)
   a(0) = 25
   a(n) = numerator of the n‑th convergent in the continued‑fraction expansion of sqrt(641) */
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D  = 641ULL;        /* the number under the square root */
    const uint64_t A0 = 25ULL;         /* floor(sqrt(D)) */

    /* p_{-1}=1, p_0=a0 */
    seq_t p_prev2 = (seq_t)1;          /* numerator of previous‑previous convergent */
    seq_t p_prev1 = (seq_t)A0;         /* numerator of previous convergent */

    if (n == 0)
        return p_prev1;                /* a(0) = 25 */

    /* continued fraction state variables */
    uint64_t m = 0;                    /* m_0 */
    uint64_t d = 1;                    /* d_0 */
    uint64_t a = A0;                   /* a_0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* next continued‑fraction coefficient */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (A0 + m_next) / d_next;

        /* convergent numerator recurrence: p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p_cur = (seq_t)a_next * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return p_prev1; /* p_n */
}

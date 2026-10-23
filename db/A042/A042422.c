/*
 * A042422  Numerators of continued fraction convergents to sqrt(739).
 * https://oeis.org/A042422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 18, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(739) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The continued fraction of sqrt(739): a0 = 27, period generated on the fly.
       The numerators p_k satisfy p_{-2}=0, p_{-1}=1, p_k = a_k * p_{k-1} + p_{k-2}. */

    const uint64_t D   = 739;   /* square‑free part under the sqrt */
    const uint64_t a0  = 27;    /* floor(sqrt(739)) */

    /* Variables for generating the continued fraction partial quotients */
    uint64_t m = 0;             /* m_0 = 0           */
    uint64_t d = 1;             /* d_0 = 1           */
    uint64_t a_cur = a0;        /* a_0 = floor(sqrt(D)) */

    /* Numerators of convergents */
    seq_t p_minus2 = 0;         /* p_{-2} */
    seq_t p_minus1 = 1;         /* p_{-1} */
    seq_t p_curr   = 0;         /* will hold p_k */

    /* Iterate from k = 0 up to n, computing p_k each time */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute current numerator */
        p_curr = (seq_t)a_cur * p_minus1 + p_minus2;

        /* Prepare for next k */
        p_minus2 = p_minus1;
        p_minus1 = p_curr;

        /* If this is the last step, break */
        if (k == n)
            break;

        /* Generate next partial quotient using the standard sqrt CF recurrence */
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Update the state variables */
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return p_curr;
}

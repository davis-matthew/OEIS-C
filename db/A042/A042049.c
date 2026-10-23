/*
 * A042049  Denominators of continued fraction convergents to sqrt(548).
 * https://oeis.org/A042049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A042049 – denominators of the convergents to sqrt(548).
    The values satisfy the linear recurrence  
        a(n) = 12166146*a(n-14) – a(n-28)   for n > 27
    but we compute them directly from the regular continued‑fraction
    expansion of sqrt(548).  The 24 initial values (n = 0 … 13) are
    1, 2, 5, 17, 22, 127, 149, 1617, 1766, 10447, 12213, 47086,
    106385, 259856.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D  = 548ULL;       /* discriminant                     */
    const uint64_t a0 = 23ULL;        /* floor(sqrt(D))                    */

    /* m, d, a are the standard continued‑fraction state variables. */
    uint64_t m = 0;              /* m(-1) */
    uint64_t d = 1;              /* d(-1) */
    uint64_t a = a0;             /* a(-1) */

    /* Convergent denominators (q) satisfy
         q(-2) = 1,  q(-1) = 0,  q(k) = a(k)*q(k-1) + q(k-2).      */
    seq_t q_pp = 1;      /* q(k-2) */
    seq_t q_p  = 0;      /* q(k-1) */

    seq_t result = 0;    /* value to return when k == n */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute current denominator q(k) */
        seq_t q_k = (seq_t)a * q_p + q_pp;
        if (k == n)
            result = q_k;

        /* Shift for next iteration */
        q_pp = q_p;
        q_p  = q_k;

        /* Compute next partial quotient a(k+1) */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    return result;
}

/*
 * A041716  Numerators of continued fraction convergents to sqrt(378).
 * https://oeis.org/A041716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041716: a(n) = numerators of continued‑fraction convergents to sqrt(378).
   Let r = ⌊√378⌋ = 19.
   Recurrence:
     p_{-1} = 1,  p_0 = r
     for k ≥ 1:
       m_k   = d_{k-1} * a_{k-1} – m_{k-1}
       d_k   = (378 − m_k * m_k) / d_{k-1}
       a_k   = (r + m_k) / d_k
       p_k   = a_k * p_{k-1} + p_{k-2}
   a(n) = p_n
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(378) */
    const uint64_t D     = 378;
    const uint64_t r     = 19;          /* floor(sqrt(378)) */
    const seq_t    a0    = r;          /* numerator of the 0th convergent */

    if (n == 0)
        return a0;

    /* convergents p_{-1} and p_0 */
    seq_t p_nm2 = 1;     /* p_{-1} */
    seq_t p_nm1 = a0;    /* p_0 */

    /* variables for continued fraction expansion */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_k = r;

    for (idx_t k = 1; k <= n; ++k) {
        /* compute the next partial quotient a_k */
        m = d * a_k - m;
        d = (D - m * m) / d;
        a_k = (r + m) / d;

        /* p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p_k = (seq_t)a_k * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_k;
    }

    return p_nm1;
}

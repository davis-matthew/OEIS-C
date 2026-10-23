/*
 * A041933  Denominators of continued fraction convergents to sqrt(489).
 * https://oeis.org/A041933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to sqrt(489):
   a(0)=1, a(1)=8, and for n≥2
   a(n)=c_{n-1}*a(n-1)+a(n-2)
   where c_k are the continued‐fraction coefficients of sqrt(489). */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* partial quotients of √489.  a0 is floor(√489)=22. */
    const uint64_t D    = 489ULL;
    const uint64_t a0   = 22ULL;
    uint64_t m = 0ULL, d = 1ULL, a_coeff = a0;

    /* q_-2 = 1, q_-1 = 0   (denominators of convergents) */
    seq_t q_nm2 = 1;   /* q_{-2} */
    seq_t q_nm1 = 0;   /* q_{-1} */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t q_i = (seq_t)a_coeff * q_nm1 + q_nm2;

        if (i == n)
            return q_i;

        /* shift the last two denominators for next iteration */
        q_nm2 = q_nm1;
        q_nm1 = q_i;

        /* Update continued fraction coefficients for next i+1 */
        uint64_t m_next = d * a_coeff - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_coeff = a_next;
    }

    /* The return above will always be reached; unused but satisfies the compiler. */
    return 0;
}

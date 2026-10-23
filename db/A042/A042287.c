/*
 * A042287  Denominators of continued fraction convergents to sqrt(669).
 * https://oeis.org/A042287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042287: Denominators of convergents to sqrt(669)
   Recurrence via continued fraction coefficients:
   Let sqrt(669) = a0 + 1/(a1 + 1/(a2 + …)).
   Use a0 = ⌊√669⌋ = 25.  Subsequent coefficients are obtained by
   the standard recurrence
        m_k = d_{k-1}*a_{k-1} - m_{k-1}
        d_k = (669 - m_k^2) / d_{k-1}
        a_k = (a0 + m_k) / d_k
   The convergents satisfy
        p_{-2}=0, p_{-1}=1;   q_{-2}=1, q_{-1}=0
        p_n = a_n * p_{n-1} + p_{n-2}
        q_n = a_n * q_{n-1} + q_{n-2}
   a(n) = q_n. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Tidy fixed constants */
    const int a0 = 25;              /* floor(sqrt(669)) */
    const int64_t D = 669;          /* argument of the square root */

    /* Convergents initialization */
    seq_t p_nm2 = (seq_t)0;  /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;  /* p_{-1} */
    seq_t q_nm2 = (seq_t)1;  /* q_{-2} */
    seq_t q_nm1 = (seq_t)0;  /* q_{-1} */

    /* Continued fraction parameters (small integers) */
    int64_t m = 0;          /* m_0   */
    int64_t d = 1;          /* d_0   */
    int64_t a = a0;         /* a_0   */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute new convergent */
        seq_t p_n = (seq_t)a * p_nm1 + p_nm2;
        seq_t q_n = (seq_t)a * q_nm1 + q_nm2;

        /* Shift for next iteration */
        p_nm2 = p_nm1;  p_nm1 = p_n;
        q_nm2 = q_nm1;  q_nm1 = q_n;

        if (k == n)        /* finished, q_n is ready */
            return q_n;

        /* Prepare next continued‑fraction coefficient a_{k+1} */
        int64_t m_next = d * a - m;
        int64_t d_next = (D - m_next * m_next) / d;
        int64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* Should never reach here */
    return (seq_t)0;
}

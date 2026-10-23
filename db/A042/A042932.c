/*
 * A042932  Numerators of continued fraction convergents to sqrt(998).
 * https://oeis.org/A042932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for numerators of convergents to sqrt(998)
   a(0) = 31
   For k ≥ 1:
       a_k = floor((31 + m_k)/d_k)
       m_k = d_{k-1} * a_{k-1} - m_{k-1}
       d_k = (998 - m_k^2) / d_{k-1}
       p_k = a_k * p_{k-1} + p_{k-2}
   where p_{-2} = 0, p_{-1} = 1.  Return p_n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long D     = 998ULL;
    const unsigned long long a0    = 31ULL;      /* floor(sqrt(998)) */

    /* Handle the initial term separately. */
    if (n == 0)
        return (seq_t)a0;

    /* State variables for continued‑fraction expansion. */
    unsigned long long m = 0ULL;       /* m_k-1  */
    unsigned long long d = 1ULL;       /* d_k-1  */
    unsigned long long a_k = a0;       /* a_k-1  */

    /* Convergent numerators. */
    seq_t p_nm2 = 0ULL;   /* p_{k-2}  (p_-2 = 0) */
    seq_t p_nm1 = 1ULL;   /* p_{k-1}  (p_-1 = 1) */
    seq_t p_k   = 0ULL;   /* p_k */

    /* k = 0 */
    p_k   = (seq_t)a_k * p_nm1 + p_nm2;  /* 31*1 + 0 = 31 */
    p_nm2 = p_nm1;
    p_nm1 = p_k;

    /* k = 1 … n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute next partial quotient a_k using the standard
           recurrence for sqrt(D). */
        unsigned long long m_next = d * a_k - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        /* Update state. */
        m = m_next;
        d = d_next;
        a_k = a_next;

        /* Compute next convergent numerator. */
        p_k = (seq_t)a_k * p_nm1 + p_nm2;

        /* Shift previous terms for next iteration. */
        p_nm2 = p_nm1;
        p_nm1 = p_k;
    }

    return p_k;
}

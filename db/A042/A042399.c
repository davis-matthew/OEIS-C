/*
 * A042399  Denominators of continued fraction convergents to sqrt(726).
 * https://oeis.org/A042399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued‑fraction convergents to sqrt(726).
 *            Computed by the standard recurrence for quadratic irrationals
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long D   = 726ULL;   /* 726 = D */
    const unsigned long long a0  = 26ULL;   /* floor(sqrt(726)) */

    /* Convergent denominators satisfy:  q[-2] = 1, q[-1] = 0,
       and  q[k] = a_k * q[k-1] + q[k-2]   (k ≥ 0).   */

    unsigned long long m   = 0;
    unsigned long long d   = 1;
    unsigned long long a_k = a0;            /* a_0 */
    seq_t q_m2 = 1;                         /* q[-2] */
    seq_t q_m1 = 0;                         /* q[-1] */
    seq_t qk   = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        qk = (seq_t)a_k * q_m1 + q_m2;       /* q_k */

        if (k == n)
            return qk;

        /* Prepare the next set of parameters (m_{k+1}, d_{k+1}, a_{k+1}) */
        unsigned long long m_next = d * a_k - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        /* Shift denominators for next iteration */
        q_m2 = q_m1;
        q_m1 = qk;

        /* Advance state */
        m    = m_next;
        d    = d_next;
        a_k  = a_next;
    }
    /* Unreachable: loop always returns */
    return 0;
}

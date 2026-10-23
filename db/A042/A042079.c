/*
 * A042079  Denominators of continued fraction convergents to sqrt(563).
 * https://oeis.org/A042079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction for sqrt(563)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   isqrt_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_int.h"

/* a(idx_t n) – denominator of the nth convergent to sqrt(563) */
seq_t a(idx_t n)
{
    const int D = 563;
    int a0 = isqrt_int(D);                 /* floor(sqrt(563)) */

    /* initial values for the continued‑fraction algorithm */
    int m = 0;
    int d = 1;
    int a_i = a0;                          /* current partial quotient */

    seq_t p_prev2 = (seq_t)0;              /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;              /* p_{-1} */
    seq_t q_prev2 = (seq_t)1;              /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;              /* q_{-1} */

    seq_t p, q;
    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute new convergent */
        p = (seq_t)a_i * p_prev1 + p_prev2;
        q = (seq_t)a_i * q_prev1 + q_prev2;

        if (i == n)
            return q;                      /* requested denominator */

        /* prepare next partial quotient */
        int m_next = d * a_i - m;
        int d_next = (D - m_next * m_next) / d;    /* always integer for quadratic surd */
        int a_next = (a0 + m_next) / d_next;

        /* shift for next iteration */
        m = m_next;
        d = d_next;
        a_i = a_next;

        p_prev2 = p_prev1; p_prev1 = p;
        q_prev2 = q_prev1; q_prev1 = q;
    }
    /* this point is never reached */
    return (seq_t)0;
}

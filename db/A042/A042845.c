/*
 * A042845  Denominators of continued fraction convergents to sqrt(953).
 * https://oeis.org/A042845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominators of continued fraction convergents to
 *            sqrt(953). Recurrence: a(0) = 1, a(1) = 1; For n ≥ 2: a(n) = c_n
 *            * a(n-1) + a(n-2), where c_n are the terms of the continued
 *            fraction of sqrt(953) (starting with 30,1,6,1,2,1,6,1,30, ...),
 *            obtained algorithmically
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t N = 953;          /* Non‑square integer */
    const uint64_t a0 = 30;          /* floor(sqrt(N)) */

    /* previous two convergents (p: numerator, q: denominator) */
    seq_t p_prevprev = 0;   /* p_{-2} */
    seq_t p_prev     = 1;   /* p_{-1} */
    seq_t q_prevprev = 1;   /* q_{-2} */
    seq_t q_prev     = 0;   /* q_{-1} */

    seq_t p = 0, q = 0;     /* current convergent */

    /* continued fraction state variables */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;                 /* a_k, starts with a0 */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute current convergent */
        p = (seq_t)a * p_prev + p_prevprev;
        q = (seq_t)a * q_prev + q_prevprev;

        if (i == n)
            return q;                 /* denominator of nth convergent */

        /* advance to next continued‑fraction term */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (N - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* shift state for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;

        p_prevprev = p_prev;
        p_prev     = p;
        q_prevprev = q_prev;
        q_prev     = q;
    }

    /* unreachable but required by the compiler */
    return (seq_t)0;
}

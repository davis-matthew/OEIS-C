/*
 * A040608  Continued fraction for sqrt(634).
 * https://oeis.org/A040608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th coefficient of the continued fraction expansion of
 *            sqrt(634)
 * Range:     u8 n <= 1, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for a(0) */
    if (n == 0)
        return (seq_t)25;          /* floor(sqrt(634)) */

    /* Constants for sqrt(634) */
    const seq_t S  = (seq_t)634;    /* the squarefree part */
    const seq_t a0 = (seq_t)25;     /* floor(sqrt(S)) */

    /* Initialize continued‑fraction variables */
    seq_t m = 0;            /* m_0 */
    seq_t d = 1;            /* d_0 */
    seq_t a_val = a0;       /* a_0 */

    /* Iterate to compute a(n) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Use 64‑bit arithmetic for intermediate products to avoid overflow */
        unsigned long long m_next_ull = (unsigned long long)d * (unsigned long long)a_val
                                        - (unsigned long long)m;
        seq_t m_next = (seq_t)m_next_ull;

        unsigned long long d_next_ull = ((unsigned long long)S
                                          - (unsigned long long)m_next * (unsigned long long)m_next)
                                        / (unsigned long long)d;
        seq_t d_next = (seq_t)d_next_ull;

        seq_t a_next = (seq_t)((a0 + m_next) / d_next);

        /* Update state for next iteration */
        m = m_next;
        d = d_next;
        a_val = a_next;
    }

    return a_val;
}

/*
 * A041801  Denominators of continued fraction convergents to sqrt(421).
 * https://oeis.org/A041801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 21, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Denominators q_n of continued‐fraction convergents to sqrt(421)
   satisfy q_{-2}=1, q_{-1}=0, q_n = a_n * q_{n-1} + q_{n-2},
   where the continued–fraction coefficients a_n come from the
   standard expansion of sqrt(421). This function returns q_n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long a0 = 20ULL; /* floor(sqrt(421)) */
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a_coef = a0;

    seq_t q_prevprev = 1; /* q_{-2} */
    seq_t q_prev     = 0; /* q_{-1} */
    seq_t q_current  = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* q_n = a_coef * q_{n-1} + q_{n-2} */
        q_current = (seq_t)a_coef * q_prev + q_prevprev;

        /* shift for next step */
        q_prevprev = q_prev;
        q_prev     = q_current;

        if (i == n) {
            break;
        }

        /* compute next continued‑fraction coefficient */
        unsigned long long m_next = d * a_coef - m;
        unsigned long long d_next = (421ULL - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_coef = a_next;
    }
    return q_current;
}

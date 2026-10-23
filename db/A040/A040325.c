/*
 * A040325  Continued fraction for sqrt(344).
 * https://oeis.org/A040325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued‑fraction coefficients of sqrt(344).
 *
 *  a(0) = floor(sqrt(344)) = 18
 *  a(n) for n > 0 is periodic: 1,1,4,1,3,1,4,1,1,36 …
 *  The period is obtained algorithmically from the standard
 *  continued‑fraction expansion of sqrt(D).
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static seq_t a0 = 0;                 /* floor(sqrt(344)) */
    static seq_t period[128] = {0};      /* period coefficients */
    static idx_t period_len = 0;         /* length of the period */

    /* compute a0 if not yet initialised */
    if (a0 == 0) {
        seq_t k = 0;
        while ((k + 1) * (k + 1) <= (seq_t)344) ++k;
        a0 = k;                           /* will be 18 */
    }

    if (n == 0)
        return a0;

    /* compute the period once */
    if (period_len == 0) {
        seq_t m = 0;                      /* m_k */
        seq_t d = 1;                      /* d_k */
        seq_t a = a0;                     /* a_k = a0 */

        while (1) {
            seq_t m_next = d * a - m;
            seq_t d_next = ((seq_t)344 - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            if (period_len >= 128)
                break;                    /* safety: should never happen */

            period[period_len++] = a_next;

            if (a_next == 2 * a0)        /* period ends */
                break;

            m = m_next;
            d = d_next;
            a = a_next;
        }
    }

    /* return appropriate term in the period */
    return period[(n - 1) % period_len];
}

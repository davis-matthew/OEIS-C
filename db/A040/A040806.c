/*
 * A040806  Continued fraction for sqrt(835).
 * https://oeis.org/A040806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction partial quotients of sqrt(835) */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a0 = 0;
    static seq_t period[200];
    static size_t period_len = 0;

    if (!initialized) {
        const seq_t D = 835u;          /* the number whose sqrt we expand */

        /* compute floor(sqrt(D)) */
        a0 = 0;
        for (seq_t t = 0; (t + 1) * (t + 1) <= D; ++t)
            a0 = t + 1;

        /* generate the repeating part of the continued fraction */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_curr = a0;
        size_t i = 0;
        while (1) {
            seq_t m_next = d * a_curr - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[i++] = a_next;
            m = m_next;
            d = d_next;
            a_curr = a_next;
            if (d == 1)         /* the period ends when d becomes 1 */
                break;
        }
        period_len = i;
        initialized = true;
    }

    if (n == 0)
        return a0;               /* first term */

    /* subsequent terms repeat every period_len */
    idx_t idx = n - 1;
    idx_t mod = idx % period_len;   /* n-1 because a0 already handled */
    return period[(size_t)mod];
}

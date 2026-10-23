/*
 * A040808  Continued fraction for sqrt(837).
 * https://oeis.org/A040808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion of sqrt(837) */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t a0 = 28;          /* floor(sqrt(837)) */
    static seq_t period[64];            /* periodic part after a0 */
    static idx_t period_len = 0;        /* length of period */

    /* compute the period only once */
    if (period_len == 0) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;
        seq_t a_next;

        /* Generate the periodic part until the repeating term 2*a0 is reached */
        while (1) {
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (837 - m_next * m_next) / d;
            a_next = (a0 + m_next) / d_next;

            period[period_len++] = a_next;   /* store term in period */

            if (a_next == 2 * a0)           /* last term of period */
                break;

            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
    }

    if (n == 0)
        return a0;
    else {
        idx_t idx = (n - 1) % period_len;
        return period[idx];
    }
}

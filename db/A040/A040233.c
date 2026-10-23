/*
 * A040233  Continued fraction for sqrt(249).
 * https://oeis.org/A040233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the simple continued fraction terms for √249.
   The first term a(0) is the integer part floor(√249)=15.
   The subsequent terms repeat with period 16.  The period is generated
   once using the standard recurrence:
       m_{k+1} = d_k * a_k - m_k
       d_{k+1} = (249 - m_{k+1}^2) / d_k
       a_{k+1} = ⌊(15 + m_{k+1}) / d_{k+1}⌋
   The period ends when a_{k+1} equals 2·15 = 30.
   After that the sequence cycles. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* First term */
    if (n == 0) {
        return (seq_t)15;
    }

    /* Periodic part of the continued fraction for √249 */
    /* Static initialization: compute once */
    static bool init = false;
    static seq_t period[32];
    static idx_t period_len = 0;

    if (!init) {
        const idx_t D      = 249;   /* the radicand */
        const idx_t a0     = 15;    /* floor(√249) */

        idx_t m = 0;
        idx_t d = 1;
        idx_t a_cur = a0;

        idx_t len = 0;
        do {
            idx_t m_next = d * a_cur - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            period[len++] = (seq_t)a_next;

            m = m_next;
            d = d_next;
            a_cur = a_next;
        } while (a_cur != 2 * a0);  /* stops after the 30 */

        period_len = len;
        init = true;
    }

    /* Return the proper term from the cyclic period */
    idx_t pos = (n - 1) % period_len;
    return period[(size_t)pos];
}

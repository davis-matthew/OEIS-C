/*
 * A041772  Numerators of continued fraction convergents to sqrt(407).
 * https://oeis.org/A041772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(407)
   Implementation: a(0) = 20
   a(n) = coeff[(n-1) % period_len] * a(n-1) + a(n-2)
   where coeff = [5,1,2,1,5,40] (period_len = 6). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(407) */
    const idx_t D    = 407;
    const idx_t a0   = 20;          /* floor(sqrt(407)) */

    /* static period storage */
    static uint16_t period[128];
    static idx_t   period_len = 0;
    static bool    initialized = false;

    if (!initialized) {
        idx_t m = 0, d = 1, a = a0;
        idx_t m_next, d_next, a_next;
        while (1) {
            m_next = d * a - m;
            d_next = (D - m_next * m_next) / d;
            a_next = (a0 + m_next) / d_next;
            period[period_len++] = (uint16_t)a_next;
            if (a_next == 2 * a0)   /* end of period */
                break;
            m = m_next;
            d = d_next;
            a = a_next;
        }
        initialized = true;
    }

    if (n == 0)
        return (seq_t)a0;            /* first numerator */

    seq_t prev2 = 1;                 /* u_{-1} */
    seq_t prev1 = (seq_t)a0;         /* u_0   */
    for (idx_t i = 1; i <= n; ++i) {
        uint16_t coeff = period[(i - 1) % period_len];
        seq_t cur = (seq_t)coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

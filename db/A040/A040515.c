/*
 * A040515  Continued fraction for sqrt(539).
 * https://oeis.org/A040515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for √539.
   a(0)=23, periodic with period 8: 4,1,1,1,1,1,4,46. */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t a0seq = 0;
    static seq_t cycle[128];
    static size_t period = 0;

    if (!initialized)
    {
        const idx_t D = 539;
        a0seq = floor_sqrt((seq_t)D);      /* a0 = ⌊√539⌋ */
        idx_t a0 = (idx_t)a0seq;

        idx_t m = 0;
        idx_t d = 1;
        idx_t a_curr = a0;
        size_t i = 0;

        for (;;)
        {
            idx_t m_next = d * a_curr - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            cycle[i++] = (seq_t)a_next;

            if (a_next == (idx_t)(2 * a0))
            {
                period = i;
                break;
            }

            m = m_next;
            d = d_next;
            a_curr = a_next;
        }
        initialized = 1;
    }

    if (n == 0)
        return a0seq;
    return cycle[(size_t)((n - 1) % period)];
}

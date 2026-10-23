/*
 * A040726  Continued fraction for sqrt(754).
 * https://oeis.org/A040726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=27; a(n)=a(n-9) for n≥1 (periodic continued fraction of
 *            √754)
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t cycle[9];
    static bool init = false;
    if (!init)
    {
        const seq_t D = (seq_t)754;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a0 = floor_sqrt(D);
        seq_t a = a0;
        int i;
        for (i = 0; i < 9; ++i)
        {
            seq_t m_next = d * a - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;
            cycle[i] = a_next;
            m = m_next;
            d = d_next;
            a = a_next;
        }
        init = true;
    }

    if (n == 0)
        return floor_sqrt((seq_t)754);   /* 27 */
    else
        return cycle[(idx_t)(n - 1) % 9];
}

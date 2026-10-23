/*
 * A040226  Continued fraction for sqrt(242).
 * https://oeis.org/A040226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction for sqrt(242). a(0)=15. For n>=1, a(n)
 *            repeats with period 10: 1,1,3,1,14,1,3,1,1,30
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* build the period once */
    static seq_t period[20];
    static idx_t period_len = 0;
    if (period_len == 0) {
        const idx_t D = 242;
        const idx_t a0 = 15;          /* floor(sqrt(242)) */

        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        /* Generate period until a == 2*a0, then include this a */
        while (1) {
            /* next terms of the continued‑fraction expansion */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
            if (a == 2 * a0)       /* end of period */
                break;
        }
    }

    if (n == 0) {
        return (seq_t)15;
    }

    /* periodic part */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}

/*
 * A040389  Continued fraction for sqrt(410).
 * https://oeis.org/A040389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the nth partial quotient of the continued fraction
 *            expansion of sqrt(410)
 * Range:     u8 n <= 0, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* constant N = 410 */
    const seq_t N = 410;
    const seq_t a0 = 20;               /* floor(sqrt(410)) */
    static bool initialized = false;
    static seq_t period[64];
    static idx_t period_len = 0;

    if (n == 0)
        return a0;

    /* compute the periodic part once */
    if (!initialized) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a  = a0;
        period_len = 0;
        do {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
        } while (a != 2 * a0);   /* 2*a0 marks the end of the period */
        initialized = true;
    }

    /* n>=1: map to periodic part */
    return period[(n - 1) % period_len];
}

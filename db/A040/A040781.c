/*
 * A040781  Continued fraction for sqrt(810).
 * https://oeis.org/A040781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction expansion of sqrt(810). a(0)=28. For n ≥ 1
 *            the terms repeat with period 8: [2,5,1,4,1,5,2,56]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    const idx_t D   = 810;
    const idx_t a0  = 28;                /* floor(sqrt(D)) */
    static idx_t period_len = 0;
    static seq_t period[16];            /* enough for the period */

    /* Compute the periodic part once */
    if (period_len == 0) {
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0);          /* a == 2*a0 terminates the period */
    }

    /* a(0) */
    if (n == 0) {
        return (seq_t)a0;
    }

    /* a(n) for n >= 1: use the periodic part */
    return period[(n - 1) % period_len];
}

/*
 * A040225  Continued fraction for sqrt(241).
 * https://oeis.org/A040225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(241).  a(0)=15, then the periodic
   continued fraction terms repeat with period length 17. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t D = 241;
    const idx_t a0 = 15;                /* floor(sqrt(241)) */

    /* Cache the periodic part of the continued fraction. */
    static seq_t period[64];
    static bool initialized = false;
    static size_t period_len = 0;

    if (!initialized) {
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_curr = a0;
        size_t i = 0;

        /* Generate the period until the term equals 2*a0. */
        do {
            m = d * a_curr - m;
            d = (D - m * m) / d;
            a_curr = (a0 + m) / d;

            period[i++] = (seq_t)a_curr;
        } while (a_curr != 2 * a0);   /* 2*a0 == 30 for D = 241 */

        period_len = i;
        initialized = true;
    }

    if (n == 0)
        return (seq_t)a0;

    /* Determine the position within the period. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}

/*
 * A040318  Continued fraction for sqrt(337).
 * https://oeis.org/A040318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(337). a(0)=floor(sqrt(337)).
   Subsequent terms repeat the period of the continued fraction
   expansion of sqrt(337).  The period is generated algorithmically
   and used for all further indices. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* constant part of the expansion */
    const uint32_t A0 = 18;          /* floor(sqrt(337)) */

    /* statically store the periodic part once */
    static seq_t period[64];
    static uint16_t period_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* generate the period of sqrt(337) */
        uint32_t m = 0, d = 1, a_curr = A0;
        uint32_t idx = 0;

        do {
            m = d * a_curr - m;
            d = (337U - m * m) / d;
            a_curr = (A0 + m) / d;
            period[idx++] = (seq_t)a_curr;
        } while (a_curr != 2 * A0);   /* period ends when a = 2 * floor(sqrt) */

        period_len = (uint16_t)idx;
        initialized = true;
    }

    if (n == 0) {
        return (seq_t)A0;
    } else {
        /* n-1 because the period starts with a(1) */
        idx_t pos = (n - 1) % period_len;
        return period[pos];
    }
}

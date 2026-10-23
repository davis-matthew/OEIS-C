/*
 * A040561  Continued fraction for sqrt(586).
 * https://oeis.org/A040561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: partial quotients of the continued fraction for sqrt(586).
   a(0) = 24, then the terms repeat with period 23:
   (4,1,4,1,1,2,1,2,7,1,2,2,1,7,2,1,2,1,1,4,1,4,48). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction (without the leading 24). */
    static bool initialized = false;
    static seq_t period[100];       /* sufficient size for the period */
    static idx_t period_len = 0;
    static const idx_t a0 = 24;     /* floor(sqrt(586)) */

    if (!initialized) {
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;
        idx_t i = 0;
        /* Compute the periodic part until a == 2*a0 (which signals termination). */
        while (1) {
            m = d * a - m;
            d = (586 - m * m) / d;
            a = (a0 + m) / d;
            period[i++] = (seq_t)a;          /* store as seq_t for output */
            if (a == 2 * a0) {
                period_len = i;
                break;
            }
        }
        initialized = true;
    }

    if (n == 0)
        return (seq_t)a0;

    /* n >= 1: use the periodic part.  Use modulo to handle large n. */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}

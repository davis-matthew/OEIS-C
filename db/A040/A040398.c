/*
 * A040398  Continued fraction for sqrt(419).
 * https://oeis.org/A040398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion of √419.
   a(0) = 20.
   The period after the first term has length 18:
   2, 7, 1, 2, 3, 1, 2, 1, 19, 1, 2, 1, 3, 2, 1, 7, 2, 40   */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[64];
    static size_t period_len;
    static seq_t a0;

    if (!initialized) {
        /* compute floor(sqrt(419)) without floating point */
        a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= 419) {
            a0++;
        }

        idx_t D = 419;
        idx_t m = 0;
        idx_t d = 1;
        idx_t a0_idx = (idx_t)a0;
        idx_t a_val = a0_idx;

        /* generate the period of the continued fraction */
        do {
            m = d * a_val - m;
            d = (D - m * m) / d;
            a_val = (a0_idx + m) / d;
            period[period_len++] = (seq_t)a_val;
        } while (a_val != (2 * a0_idx));

        initialized = true;
    }

    if (n == 0) {
        return a0;
    }

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}

/*
 * A040851  Continued fraction for sqrt(881).
 * https://oeis.org/A040851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction terms for sqrt(881)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(881) */
    static int initialized = 0;
    static seq_t period[30];
    static size_t period_len = 0;

    if (!initialized) {
        const uint64_t D = 881;
        const uint64_t a0 = 29;   /* floor(sqrt(881)) */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_curr = a0;
        size_t idx = 0;

        /* Build the period.  The period ends when a_curr == 2*a0. */
        while (1) {
            m = d * a_curr - m;
            d = (D - m * m) / d;
            a_curr = (a0 + m) / d;
            period[idx++] = (seq_t)a_curr;
            if (a_curr == 2 * a0) {
                period_len = idx;
                break;
            }
        }
        initialized = 1;
    }

    if (n == 0)
        return (seq_t)29;   /* a(0) = floor(sqrt(881)) */

    idx_t idx = (n - 1) % (idx_t)period_len;
    return period[(size_t)idx];
}

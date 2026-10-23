/*
 * A040697  Continued fraction for sqrt(724).
 * https://oeis.org/A040697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(724).  a(0)=26; thereafter periodic of period 42. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t D   = 724;
    static const seq_t a0  = 26;
    static seq_t period[200];
    static size_t period_len = 0;

    if (n == 0)
        return a0;

    if (period_len == 0) {
        seq_t m      = 0;
        seq_t d      = 1;
        seq_t a_prev = a0;

        while (1) {
            m = d * a_prev - m;
            d = (D - m * m) / d;
            seq_t a_now = (a0 + m) / d;
            period[period_len++] = a_now;
            if (a_now == a0 * (seq_t)2)
                break;
            a_prev = a_now;
        }
    }

    size_t idx = (size_t)((n - 1) % (idx_t)period_len);
    return period[idx];
}

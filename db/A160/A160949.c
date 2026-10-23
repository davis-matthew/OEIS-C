/*
 * A160949  a(n) = n - digital sum(n+1).
 * https://oeis.org/A160949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign, easy
 * Formula:   a(n) = n - digital_sum(n+1)
 * Range:     i8 n <= 137, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;      /* value to sum digits of */
    seq_t ds = 0;                /* running digit sum */

    while (m > 0) {
        ds += m % 10;            /* add last digit */
        m /= 10;                 /* drop last digit */
    }

    return (seq_t)n - ds;        /* result may be negative */
}

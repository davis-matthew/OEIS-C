/*
 * A146306  a(n) = numerator of (n-6)/(2n).
 * https://oeis.org/A146306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = numerator of (n-6)/(2n)
 * Range:     i8 n <= 63, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute reduced numerator of (n-6)/(2n) */
    seq_t num = (seq_t)n - 6;    /* signed number, may be negative for n<6 */
    seq_t den = 2 * (seq_t)n;    /* always positive */

    seq_t abs_num = num >= 0 ? num : -num;
    seq_t g = gcd(abs_num, den);

    return num / g;
}

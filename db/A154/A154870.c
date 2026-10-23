/*
 * A154870  Period 6: repeat [7, 5, 1, -7, -5, -1].
 * https://oeis.org/A154870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=7, a(1)=5, a(2)=1; a(n) = -a(n-3) for n>2 (period 6)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* period of length 6 */
    static const seq_t pat[6] = {7, 5, 1, -7, -5, -1};

    /* compute the index within the period */
    size_t idx = (size_t)(n % 6ULL);

    return pat[idx];
}

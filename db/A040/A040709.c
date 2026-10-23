/*
 * A040709  Continued fraction for sqrt(737).
 * https://oeis.org/A040709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction coefficient of sqrt(737), with a(0) =
 *            27 and periodic part of length 14 thereafter
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[14] = {6, 1, 3, 3, 7, 2, 4, 2, 7, 3, 3, 1, 6, 54};
    if (n == 0)
        return 27;
    return period[(size_t)((n - 1) % 14)];
}

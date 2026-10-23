/*
 * A021232  Decimal expansion of 1/228.
 * https://oeis.org/A021232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = 0 for n = 0,1; otherwise a(n) = period[(n-2) % 18] where
 *            period = {4, 3, 8, 5, 9, 6, 4, 9, 1, 2, 2, 8, 0, 7, 0, 1, 7, 5}.
 *            This yields the decimal digits of 1/228
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    static const seq_t period[18] = {
        4, 3, 8, 5, 9, 6, 4, 9,
        1, 2, 2, 8, 0, 7, 0, 1,
        7, 5
    };

    return period[(size_t)((n - 2) % 18)];
}

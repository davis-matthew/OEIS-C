/*
 * A021598  Decimal expansion of 1/594.
 * https://oeis.org/A021598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/594, with a(0) = 0,
 *            and for n ≥ 1, a(n) = {0,1,6,8,3,5}[(n-1) mod 6]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static const seq_t digits[6] = {0, 1, 6, 8, 3, 5};
    idx_t r = (n - 1) % 6;
    return digits[r];
}

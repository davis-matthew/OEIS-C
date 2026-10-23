/*
 * A021209  Decimal expansion of 1/205.
 * https://oeis.org/A021209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0, if n < 2; a(n) = {4,8,7,8,0}[(n-2) mod 5], otherwise
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }
    static const seq_t period[5] = {4, 8, 7, 8, 0};
    return period[(size_t)(n - 2) % 5];
}

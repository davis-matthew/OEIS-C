/*
 * A037589  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,1.
 * https://oeis.org/A037589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   * a(n) = 10 * a(n-1) + d(n), where d(n) = 3 if n is odd, else 1
 *            * a(1) = 3 * * This directly builds the decimal number digit by
 *            digit
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        result = result * 10 + ((i & 1) ? 3 : 1);
    }
    return result;
}

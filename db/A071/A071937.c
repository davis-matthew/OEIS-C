/*
 * A071937  Reverse(n)!.
 * https://oeis.org/A071937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(n)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t temp = (uint64_t)n;
    uint64_t rev  = 0;

    /* Reverse the decimal digits of n */
    while (temp != 0) {
        rev = rev * 10 + (temp % 10);
        temp /= 10;
    }

    /* Compute factorial of rev */
    seq_t result = 1;
    for (uint64_t i = 2; i <= rev; ++i) {
        result = result * (seq_t)i;
    }
    return result;
}

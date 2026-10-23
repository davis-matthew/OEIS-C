/*
 * A030101  a(n) is the number produced when n is converted to binary digits,
 *          the binary digits are reversed and then converted back into a
 *          decimal number.
 * https://oeis.org/A030101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac, nice, hear, look
 * Formula:   a(n) = the integer represented by the binary digits of n in
 *            reverse order (i.e. reverse(n in binary))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rev = 0;
    /* n is of type idx_t; it is safe to shift and mask it */
    while (n > 0) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return rev;
}

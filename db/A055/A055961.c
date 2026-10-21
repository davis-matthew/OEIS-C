/*
 * A055961  a(n) = n - (reversal of base-11 digits of n) (written in base 10).
 * https://oeis.org/A055961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, look
 * Formula:   a(n) = n - reverse_base11(n)
 * Range:     i8 n <= 123, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t temp = n;
    while (temp > 0) {
        uint64_t digit = temp % 11;
        rev = rev * (seq_t)11 + (seq_t)digit;
        temp /= 11;
    }
    return (seq_t)n - rev;
}

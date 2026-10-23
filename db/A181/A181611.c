/*
 * A181611  Position of rightmost zero in 2^n (including leading zero).
 * https://oeis.org/A181611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the rightmost zero digit in the decimal *
 *            representation of 2^n, counting the least significant digit as
 *            position 0 * and including a leading zero after the most
 *            significant digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2^n has at most about 0.30103*n decimal digits.  For n up to
     * roughly 10 000 this is < 3011 digits; use a static array that
     * comfortably exceeds this.  The array stores digits least
     * significant first. */
    static uint8_t digits[4000];
    size_t len = 1;
    digits[0] = 1;                 /* 2^0 = 1 */

    /* Multiply by 2, n times. */
    for (idx_t i = 0; i < n; ++i)
    {
        unsigned int carry = 0;
        for (size_t j = 0; j < len; ++j)
        {
            unsigned int tmp = (unsigned int)digits[j] * 2 + carry;
            digits[j] = (uint8_t)(tmp % 10);
            carry     = tmp / 10;
        }
        while (carry)
        {
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    /* Scan for the first zero starting from the least significant digit. */
    for (size_t pos = 0; pos < len; ++pos)
        if (digits[pos] == 0)
            return (seq_t)pos;

    /* No zero in the decimal expansion; the implicit leading zero is
     * at position len. */
    return (seq_t)len;
}

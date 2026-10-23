/*
 * A024637  n written in fractional base 6/4.
 * https://oeis.org/A024637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 371, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = representation of n in the fractional base 6/4.
 * The digits are 0..5 and are written in base‑10 as the number
 * obtained by concatenating those digits.
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const seq_t BASE_A = 6;   /* digits range 0..5 */
    const seq_t BASE_B = 4;   /* carry factor */

    /* Maximum number of digits needed for typical inputs (n up to 10⁶) is well below 128 */
    uint8_t digits[128];
    size_t   len = 0;

    seq_t cur = (seq_t)n;
    while (cur > 0)
    {
        seq_t digit = cur % BASE_A;
        digits[len++] = (uint8_t)digit;          /* store least significant digit first */
        cur = (cur - digit) / BASE_A * BASE_B;   /* next value after removing the digit */
    }

    /* Build the final integer from the most significant digit first */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
    {
        result = result * 10 + (seq_t)digits[len - 1 - i];
    }

    return result;
}

/*
 * A024639  n written in fractional base 7/2.
 * https://oeis.org/A024639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n written in base 7/2 using digits 0–6, with a carry of 2
 *            when a digit exceeds 6
 * Range:     u8 n <= 47, u16 n <= 1189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* digits are stored least‑significant first */
    uint8_t digits[128];
    size_t len = 0;

    idx_t cur = n;
    while (cur > 0) {
        /* Find the least‑significant digit.
           rem = (2*cur) mod 7; a = (4*rem) mod 7   (since 2⁻¹ ≡ 4 (mod 7))  */
        uint64_t rem = ( (cur % 7) * 2 ) % 7;           // 0..6
        uint64_t a_digit = (4 * rem) % 7;               // 0..6

        digits[len++] = (uint8_t)a_digit;

        /* Advance to the next higher place.
           cur' = (cur - a_digit) * 2 / 7   (exact division)          */
        __uint128_t tmp = ( (__uint128_t)(cur - a_digit) ) * 2;
        cur = (idx_t)(tmp / 7);
    }

    /* Convert digit array to a decimal number */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        /* multiply by 10 and add the most‑significant remaining digit */
        result = result * 10 + digits[len - 1 - i];
    }
    return result;
}

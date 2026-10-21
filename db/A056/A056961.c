/*
 * A056961  Base 12 reversal of n (written in base 10).
 * https://oeis.org/A056961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse the base‑12 digits of n and interpret the
 *            reversed * number as base‑12; i.e. write n in base 12, reverse
 *            the digit string, * then convert back to decimal
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Extract base‑12 digits (least significant first). */
    uint8_t digits[64];
    uint8_t count = 0;
    while (n > 0) {
        digits[count++] = (uint8_t)(n % 12);
        n /= 12;
    }

    /* Build the reversed number by interpreting the digits array as a
     * base‑12 number with digits in the same order as extracted. */
    seq_t rev = 0;
    for (uint8_t i = 0; i < count; ++i) {
        rev = rev * 12 + digits[i];
    }
    return rev;
}

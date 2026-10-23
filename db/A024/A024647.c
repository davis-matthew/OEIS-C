/*
 * A024647  n written in fractional base 8/5.
 * https://oeis.org/A024647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal integer obtained by concatenating the digits of n
 *            in base 8/5 (digits 0–7)
 * Range:     u8 n <= 15, u16 n <= 95, u32 n <= 671, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* The number of digits needed for n up to 64‑bit is at most 94,
       so 256 slots is more than enough. */
    unsigned char digits[256];
    size_t pos = 0;

    while (n > 0) {
        /* Least significant digit in base 8/5 is n mod 8. */
        digits[pos++] = (unsigned char)(n % 8);
        /* Move to the next higher position: n := 5 * floor(n/8). */
        n = (n / 8) * 5;
    }

    /* Build the decimal integer from most significant digit to least. */
    seq_t result = 0;
    for (size_t i = 0; i < pos; ++i) {
        result = result * 10 + digits[pos - 1 - i];
    }
    return result;
}

/*
 * A056960  Base 11 reversal of n (written in base 10).
 * https://oeis.org/A056960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = reverse the base‑11 representation of n * (treating n as
 *            a decimal integer, then converting to base 11, * reversing the
 *            digits of that base‑11 number, and interpreting * the reversed
 *            string back as a base‑11 number)
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Store the base‑11 digits of n in little‑endian order. */
    uint64_t tmp = n;          /* idx_t is uint64_t, safe to use locally */
    unsigned char digits[64];  /* plenty of room for any 64‑bit value */
    size_t len = 0;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 11);
            digits[len++] = (unsigned char)d;
            tmp /= 11;
        }
    }

    /* Interpret the digits array as a base‑11 number (i.e., reverse
     * the original base‑11 representation). */
    seq_t rev = 0;
    for (size_t i = 0; i < len; ++i) {
        rev = rev * (seq_t)11 + (seq_t)digits[i];
    }
    return rev;
}

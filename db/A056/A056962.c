/*
 * A056962  Base 16 reversal of n (written in base 10).
 * https://oeis.org/A056962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = reverse hexadecimal representation of n, interpreted as a
 *            base‑10 integer
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t x = n;
    while (x != 0) {
        seq_t digit = (seq_t)(x % 16);
        rev = rev * 16 + digit;
        x /= 16;
    }
    return rev;
}

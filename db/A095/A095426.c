/*
 * A095426  a(n) = 11 written in base n.
 * https://oeis.org/A095426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer whose decimal representation is the
 *            base‑n representation of decimal 11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Base‑1 (unary) representation: eleven 1’s */
    if (n == 1) {
        return (seq_t)11111111111ull;
    }

    seq_t base = (seq_t)n;
    seq_t value = 11;

    /* Digits of 11 in base n, least significant first */
    int digits[64];
    int dpos = 0;
    while (value != 0) {
        digits[dpos++] = (int)(value % base);
        value /= base;
    }

    seq_t result = 0;
    for (int i = dpos - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}

/*
 * A024642  n written in fractional base 7/5.
 * https://oeis.org/A024642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = representation of n in fractional base 7/5 (digits 0..6,
 *            carry 5 when digit exceeds 6)
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 195, u64 n <= 5900,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    const seq_t a_base = 7;   /* a in fractional base a/b */
    const seq_t b_factor = 5; /* b in fractional base a/b */

    /* store least‑significant digits first */
    unsigned char digits[64];
    int len = 0;
    seq_t x = (seq_t)n;

    while (x != 0) {
        seq_t d = x % a_base;        /* least significant digit (0..6)  */
        digits[len++] = (unsigned char)d;
        x = (x - d) * b_factor / a_base;
    }

    /* build decimal integer from most‑significant to least‑significant */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}

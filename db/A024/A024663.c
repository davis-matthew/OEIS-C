/*
 * A024663  n written in fractional base 10/8.
 * https://oeis.org/A024663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 149, u64 n <= 1479,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n written in fractional base 10/8; the digits are concatenated as a decimal integer. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    uint8_t digits[128];
    size_t len = 0;
    idx_t cur = n;

    while (cur > 0) {
        uint8_t d = (uint8_t)(cur % 10);
        digits[len++] = d;
        cur = ((cur - d) / 10) * 8;
    }

    seq_t ans = 0;
    for (size_t i = len; i > 0; --i) {
        ans = ans * (seq_t)10 + digits[i - 1];
    }
    return ans;
}

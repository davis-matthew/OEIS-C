/*
 * A217009  Multiples of 7 in base 8.
 * https://oeis.org/A217009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal value of the octal representation of 7 · n
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0u;               /* sequence offset is 1 */

    seq_t m = (seq_t)n * 7u;      /* 7 · n */

    /* Extract octal digits (least‑significant first) */
    uint8_t digits[64];
    idx_t len = 0;
    while (m != 0u) {
        digits[len++] = (uint8_t)(m % 8u);
        m /= 8u;
    }

    /* Build the decimal number formed by those digits */
    seq_t res = 0u;
    for (idx_t i = len; i > 0; --i) {
        res = res * 10u + (seq_t)digits[i - 1];
    }

    return res;
}

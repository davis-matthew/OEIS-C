/*
 * A024645  n written in fractional base 8/3.
 * https://oeis.org/A024645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = representation of n in fractional base 8/3 (digits 0–7;
 *            “subtract 8 and carry 3” when a digit would exceed 7). The value
 *            is the decimal number read from those digits
 * Range:     u8 n <= 23, u16 n <= 447, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    unsigned char digits[70];
    int len = 0;

    /* Break n into digits of the fractional base 8/3.
       Least‑significant digit first. */
    while (cur != (seq_t)0) {
        seq_t d = cur % (seq_t)8;           /* least‑significant digit */
        digits[len++] = (unsigned char)d;   /* 0 ≤ d ≤ 7 */
        cur = (3*cur - 3*d) / (seq_t)8;     /* carry 3 on overflow */
    }

    /* Build the decimal integer from most significant digit. */
    seq_t res = (seq_t)0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * (seq_t)10 + (seq_t)digits[i];
    }

    return res;
}

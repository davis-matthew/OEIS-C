/*
 * A024650  n written in fractional base 9/2.
 * https://oeis.org/A024650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 3041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is n written in fractional base 9/2 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* digits[0] is the least–significant digit */
    unsigned char digits[64];
    size_t len = 0;
    while (n != 0) {
        idx_t d = n % 9;           /* digit 0..8 */
        digits[len++] = (unsigned char)d;
        n = (n / 9) * 2;           /* move to the next place */
    }

    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        res = res * 10 + (seq_t)digits[len - 1 - i];
    }
    return res;
}

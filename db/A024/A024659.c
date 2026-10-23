/*
 * A024659  n written in fractional base 10/4.
 * https://oeis.org/A024659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = representation of n in base 10/4, read as a decimal
 *            integer. The digits are obtained by repeatedly taking d = n % 10
 *            and reducing n by (n - d)/10 * 4 until n becomes zero. The
 *            resulting digits are then assembled into a decimal integer
 * Range:     u8 n <= 29, u16 n <= 329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* a(0) = 0 */
        return 0;

    /* store least‑significant digits first */
    unsigned char digits[64];
    size_t pos = 0;

    seq_t m = (seq_t)n;
    while (m != 0) {
        /* digit in base 10 */
        seq_t d = m % 10;               /* 0 <= d <= 9 */
        digits[pos++] = (unsigned char)d;

        /* n = (n - d) / 10 * 4   (exact division because n % 10 == d) */
        m = ((m - d) / 10) * 4;
    }

    /* assemble digits into a decimal integer, most significant first */
    seq_t result = 0;
    while (pos--) {
        result = result * 10 + digits[pos];
    }
    return result;
}

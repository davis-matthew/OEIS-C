/*
 * A140267  Nonnegative integers in balanced ternary representation (with 2
 *          standing for -1 digit).
 * https://oeis.org/A140267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = balanced ternary representation of n, using digits
 *            {0,1,2} where 2 stands for -1. The resulting digits are
 *            concatenated as a base‑10 number
 * Range:     u8 n <= 13, u16 n <= 121, u32 n <= 9715, u64 n <= 9715,
 *            u128 n <= 9715, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    idx_t k = n;
    /* Maximum digits for 64‑bit n is < 43, so 64 is ample. */
    unsigned char digits[64];
    size_t len = 0;

    while (k > 0) {
        idx_t r = k % 3;
        k /= 3;
        int d;
        if (r == 0) {
            d = 0;
        } else if (r == 1) {
            d = 1;
        } else { /* r == 2, represents -1 */
            d = -1;
            k += 1;          /* carry to next digit */
        }
        digits[len++] = (unsigned char)(d == -1 ? 2 : d);
    }

    seq_t val = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char d = digits[len - 1 - i]; /* MSD first */
        val = val * 10 + (seq_t)d;
    }
    return val;
}

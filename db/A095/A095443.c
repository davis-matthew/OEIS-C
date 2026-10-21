/*
 * A095443  a(n) = 25 written in base 12 - n.
 * https://oeis.org/A095443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the decimal integer whose decimal digits are the
 *            base-(12‑n) representation of 25. * For base = 1 (i.e. n = 11)
 *            the representation is 25 ones (unary). For any other n giving a
 *            base < 2 the value is not defined; * this implementation returns
 *            0 in that case
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the base.  For n <= 10 the base is >= 2, for n = 11 it is 1. */
    int base = 12 - (int)n;

    /* Unsupported bases (<=0) return 0. */
    if (base < 1) return (seq_t)0;

    /* Unary representation (base = 1): 25 ones. */
    if (base == 1)
    {
        seq_t r = 0;
        for (int i = 0; i < 25; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* Convert the fixed value 25 to the desired base (2‑12). */
    int val = 25;
    int digits[32];
    int idx = 0;
    while (val > 0)
    {
        digits[idx++] = val % base;
        val /= base;
    }

    /* Build the decimal integer from the most‑significant digit to the least. */
    seq_t res = 0;
    for (int i = idx - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}

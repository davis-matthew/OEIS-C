/*
 * A240237  Concatenated digits of pyramidalized numbers: Write the decimal
 *          digits of n (a nonnegative integer) and take successive absolute
 *          differences ("pyramidalization"), then concatenate, from left to
 *          right, all the digits in each level of the pyramid, and finally
 *          concatenate those results for all level...
 * https://oeis.org/A240237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   concatenated digits of pyramidalized number
 * Range:     u8 n <= 25, u16 n <= 99, u32 n <= 4294, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes A240237: concatenated digits of pyramidalized numbers. */
seq_t a(idx_t n)
{
    /* digits[0 .. d-1] holds the decimal digits of n,
       most–significant first.  d is the number of digits. */
    unsigned char digits[64];
    size_t d = 0;

    if (n == 0) {
        digits[0] = 0;
        d = 1;
    } else {
        /* fill digits in reverse order (least‑significant first) */
        idx_t tmp = n;
        while (tmp > 0) {
            digits[d++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to obtain most‑significant first */
        for (size_t i = 0; i < d / 2; ++i) {
            unsigned char c = digits[i];
            digits[i] = digits[d - 1 - i];
            digits[d - 1 - i] = c;
        }
    }

    /* build the result by concatenating digits level by level */
    seq_t result = 0;
    for (size_t i = 0; i < d; ++i)
        result = result * 10 + (seq_t)digits[i];

    /* temporary buffer for differences in the next level */
    unsigned char diff[64];
    size_t len = d;

    while (len > 1) {
        /* compute the next level of the pyramid */
        for (size_t i = 0; i < len - 1; ++i) {
            unsigned char a = digits[i];
            unsigned char b = digits[i + 1];
            diff[i] = (a > b) ? (a - b) : (b - a);
        }

        /* append the new level digits to the result */
        for (size_t i = 0; i < len - 1; ++i)
            result = result * 10 + (seq_t)diff[i];

        /* copy the computed differences back into digits for the next step */
        for (size_t i = 0; i < len - 1; ++i)
            digits[i] = diff[i];

        /* proceed to the next level */
        len -= 1;
    }

    return result;
}

/*
 * A046718  Number of permutations of [ n ] with exactly one 132-pattern and
 *          two 123-patterns.
 * https://oeis.org/A046718
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-8)*(n^3-11*n^2+54*n-88) , n ≥ 4
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 26, u64 n <= 54, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is undefined;
       return 0 for simplicity. */
    if (n < 4)
        return 0;

    /* Compute the polynomial part: n^3 - 11*n^2 + 54*n - 88 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n²   */
    seq_t n3 = n2 * nn;          /* n³   */

    seq_t p = n3 - (seq_t)11 * n2 + (seq_t)54 * nn - (seq_t)88;

    /* Adjust by a power of 2 depending on whether n ≥ 8 or not.
       For n ≥ 8 we multiply by 2^(n-8).
       For n < 8 we divide by 2^(8-n) (right shift). */
    if (n >= 8) {
        /* Multiply by 2^(n-8) by repeated left‐shift.
           Shifting one bit at a time avoids overflow for 64‑bit
           builds when the final result still fits. */
        idx_t shift = n - 8;
        while (shift--)
            p <<= 1;
    } else {
        idx_t shift = 8 - n;
        while (shift--)
            p >>= 1;
    }

    return p;
}

/*
 * A121471  a(n) = 9*n^2/4 -4*n +19/8 -3*(-1)^n/8.
 * https://oeis.org/A121471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n^2 - 32*n + 19 - 3*(-1)^n) / 8
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid integer promotion issues. */
    seq_t nn = (seq_t)n;

    /* Compute the numerator: 18*n^2 - 32*n + 19 - 3*(-1)^n.
       For even n, we subtract 3; for odd n, we add 3. */
    seq_t num = (seq_t)18 * nn * nn;   /* 18*n^2 */
    num -= (seq_t)32 * nn;             /* -32*n */
    num += (seq_t)19;                  /* +19 */

    if ((n & 1U) == 0) {               /* even n */
        num -= (seq_t)3;
    } else {                           /* odd n */
        num += (seq_t)3;
    }

    /* Division by 8 is exact for all n >= 1. */
    return num / (seq_t)8;
}

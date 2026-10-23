/*
 * A030442  Values of Newton-Gregory forward interpolating polynomial
 *          (1/6)*(4*n^4 - 60*n^3 + 347*n^2 - 927*n + 978).
 * https://oeis.org/A030442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^4 - 60*n^3 + 347*n^2 - 927*n + 978)/6, offset 0
 * Range:     u8 none, u16 n <= 15, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t for calculations */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    /* numerator of the closed‐form expression */
    seq_t num = 4 * n4 - 60 * n3 + 347 * n2 - 927 * nn + 978;
    /* the numerator is always divisible by 6, so integer division is exact */
    return num / 6;
}

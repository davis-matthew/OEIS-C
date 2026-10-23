/*
 * A174120  Partial sums of A024012.
 * https://oeis.org/A174120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1+2^n-n/6+n^2/2-n^3/3)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute: a(n) = (-1 + 2^n - n/6 + n^2/2 - n^3/3) */
    /* Equivalent integer form: a(n) = (6*2^n - n + 3*n^2 - 2*n^3 - 6) / 6 */

    seq_t pow2n   = pow2(n);                 /* 2^n */
    seq_t six_pow2n = pow2n * (seq_t)6;       /* 6*2^n */

    seq_t nn  = (seq_t)n;                    /* n */
    seq_t n2  = nn * nn;                     /* n^2 */
    seq_t n3  = n2 * nn;                     /* n^3 */

    /* Compute the numerator: 6*2^n - n + 3*n^2 - 2*n^3 */
    seq_t num = six_pow2n - nn + (seq_t)3 * n2 - (seq_t)2 * n3;

    /* Subtract the constant 6 and divide by 6 */
    seq_t result = (num - (seq_t)6) / (seq_t)6;

    return result;
}

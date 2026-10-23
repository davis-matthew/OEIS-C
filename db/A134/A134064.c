/*
 * A134064  Let P(A) be the power set of an n-element set A. Then a(n) = the
 *          number of pairs of elements {x,y} of P(A) for which either 0) x
 *          and y are intersecting but for which x is not a subset of y and y
 *          is not a subset of x, or 1) x and y are intersecting and for which
 *          either x is a proper subset of y or...
 * https://oeis.org/A134064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(4^n - 3^n + 2^n + 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* provides seq_pow(base, exp) */

seq_t a(idx_t n)
{
    seq_t pow2n = seq_pow((seq_t)2, n);   /* 2^n   */
    seq_t pow3n = seq_pow((seq_t)3, n);   /* 3^n   */
    seq_t pow4n = seq_pow((seq_t)4, n);   /* 4^n   */

    seq_t tmp = pow4n - pow3n;   /* 4^n - 3^n */
    tmp += pow2n;                /* + 2^n */
    tmp += (seq_t)1;             /* + 1   */

    return tmp / 2;              /* divide by 2 (always integer) */
}

/*
 * A229749  Number of defective 4-colorings of an n X 2 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly two mistakes, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n‑3) * (49*n² – 192*n + 195) for n > 2 a(1)=0, a(2)=3,
 *            a(3)=60 Implementation uses the helper functions pow2 and
 *            mul_small
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 50, u128 n <= 111,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Base cases directly from the sequence definition */
    if (n == 1) return 0;
    if (n == 2) return 3;
    if (n == 3) return 60;

    /* Work in seq_t arithmetic to avoid overflow of intermediate values */
    seq_t nn   = (seq_t)n;           /* n as seq_t */
    seq_t n_sq = nn * nn;            /* n² */

    /* Compute the polynomial part: 49*n² - 192*n + 195 */
    seq_t t1   = mul_small(n_sq, 49);            /* 49*n²   */
    seq_t t2   = mul_small(nn, 192);            /* 192*n   */
    seq_t poly = t1 + (seq_t)195;               /* 49*n² + 195 */
    poly -= t2;                                 /* 49*n² - 192*n + 195 */

    /* Multiply by 2^(n‑3) */
    seq_t pow_val = pow2((idx_t)(n - 3));

    return pow_val * poly;
}

/*
 * A134019  Let P(A) be the power set of an n-element set A. Then a(n) = the
 *          number of pairs of elements {x,y} of P(A) for which either 0) x
 *          and y are intersecting but for which x is not a subset of y and y
 *          is not a subset of x, or 1) x = y.
 * https://oeis.org/A134019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)(4^n - 3^(n+1) + 5*2^n - 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);          /* 4^n   */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n + 1);      /* 3^(n+1) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);          /* 2^n   */

    seq_t res = pow4;
    res += pow2 * 5;   /* + 5*2^n   */
    res -= pow3;       /* - 3^(n+1) */
    res -= 1;          /* - 1       */
    res /= 2;          /* divide by 2 */

    return res;
}

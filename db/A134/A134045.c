/*
 * A134045  Let P(A) be the power set of an n-element set A. Then a(n) = the
 *          number of pairs of elements {x,y} of P(A) for which either 0) x
 *          and y are disjoint and for which either x is a subset of y or y is
 *          a subset of x, or 1) x and y are intersecting but for which x is
 *          not a subset of y and y is not a sub...
 * https://oeis.org/A134045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)(4^n - 3^(n+1) + 7*2^n - 3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* compute 4^n */
    seq_t p4 = pow4(n);
    /* compute 3^(n+1) */
    seq_t p3 = pow3(n + 1);
    /* compute 2^n */
    seq_t p2 = pow2(n);

    /* val = 4^n - 3^(n+1) + 7*2^n - 3 */
    seq_t val = p4 - p3 + (p2 * 7) - 3;

    /* divide by 2 */
    val = val / 2;
    return val;
}

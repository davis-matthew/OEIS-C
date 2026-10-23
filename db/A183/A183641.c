/*
 * A183641  Number of (n+1) X 9 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*4^n + 1530*3^n + 37320*2^n + 223290
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   mul_small, pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t p4 = pow4(n);          /* 4^n   */
    seq_t p3 = pow3(n);          /* 3^n   */
    seq_t p2 = pow2(n);          /* 2^n   */
    seq_t result = mul_small(p4, 4);          /* 4*4^n   */
    result += mul_small(p3, 1530);           /* 1530*3^n*/
    result += mul_small(p2, 37320);          /* 37320*2^n */
    result += (seq_t)223290;                /* constant   */
    return result;
}

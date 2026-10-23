/*
 * A174420  Let A(n) = floor((3/2)^n), B(n)=3^n-2^n*A(n); then
 *          a(n)=2^n-A(n)-B(n)-2.
 * https://oeis.org/A174420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n - A(n) - B(n) - 2, where A(n) = floor((3/2)^n) = 3^n
 *            / 2^n B(n) = 3^n - 2^n * A(n) (i.e. 3^n mod 2^n) This is
 *            equivalent to a(n) = (2^n-1)*ceil((3/2)^n) - 3^n - 1
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t two   = pow2(n);   /* 2^n   */
    seq_t three = pow3(n);   /* 3^n   */

    /* A(n) = floor((3/2)^n) = 3^n div 2^n */
    seq_t A = three / two;

    /* a(n) = 2^n - A - (3^n - 2^n*A) - 2
            = 2^n - A - 3^n + 2^n*A - 2 */
    return two - A - three + two * A - 2;
}

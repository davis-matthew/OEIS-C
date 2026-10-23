/*
 * A100315  Number of 3 X n 0-1 matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1), (01;0), (10;0)
 *          and (01;1).
 * https://oeis.org/A100315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 4*n + 2 for n>0; a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = pow2(n);          /* 2^n   */
    result += (seq_t)4 * (seq_t)n;   /* + 4*n */
    result += 2;                     /* + 2   */
    return result;
}

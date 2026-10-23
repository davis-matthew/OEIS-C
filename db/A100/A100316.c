/*
 * A100316  Number of 4 X n 0-1 matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1), (01;0), (10;0)
 *          and (01;1).
 * https://oeis.org/A100316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, if n == 0; otherwise a(n) = 2^n + 6*n + 8
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t two_pow = pow2(n);           /* 2^n */
    seq_t term_mul = ((seq_t)6 * (seq_t)n); /* 6*n */
    seq_t result = two_pow + term_mul + ((seq_t)8);
    return result;
}

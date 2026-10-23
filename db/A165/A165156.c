/*
 * A165156  a(n) = n^(2*n-1)-(2*n-1)^n.
 * https://oeis.org/A165156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^(2*n-1)-(2*n-1)^n
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset starts at 1, but handle 0 safely */

    seq_t sn            = (seq_t)n;
    seq_t two_n_minus1  = (seq_t)(2*n - 1);

    seq_t term1 = big_pow(sn, two_n_minus1);         /* n^(2*n-1)   */
    seq_t term2 = big_pow(two_n_minus1, sn);        /* (2*n-1)^n   */

    return term1 - term2;                           /* a(n) */
}

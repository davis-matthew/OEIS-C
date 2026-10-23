/*
 * A024042  a(n) = 4^n - n^6.
 * https://oeis.org/A024042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^6
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;      /* n^2 */
    seq_t n4   = n2 * n2;          /* n^4 */
    seq_t n6   = n4 * n2;          /* n^6  (n^4 * n^2) */

    seq_t pow4 = big_pow((seq_t)4, (seq_t)n); /* 4^n */

    return pow4 - n6;
}

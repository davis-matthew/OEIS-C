/*
 * A153217  a(n) = n^(n+3)-(n+3)^n.
 * https://oeis.org/A153217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^(n+3) - (n+3)^n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t term1 = big_pow((seq_t)n, (seq_t)(n + 3));
    seq_t term2 = big_pow((seq_t)(n + 3), (seq_t)n);
    return term1 - term2;
}

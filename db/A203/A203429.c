/*
 * A203429  a(n) = w(n+1)/(3*w(n)), where w = A203428.
 * https://oeis.org/A203429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(-3)^(n-1) * (n+1)^n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp    = (seq_t)n;
    seq_t pow_neg3 = big_pow((seq_t)-3, exp - 1);
    seq_t factor   = -pow_neg3;
    seq_t pow_np1   = big_pow((seq_t)(n + 1), exp);
    return factor * pow_np1;
}

/*
 * A141413  Inverse binomial transform of A140962.
 * https://oeis.org/A141413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(1) = -1, a(n) = (-3)^(n-2) for n >= 2
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 21, i64 n <= 41, i128 n <= 82,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;
    return big_pow((seq_t)-3, (seq_t)(n - 2));
}

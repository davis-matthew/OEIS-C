/*
 * A141531  Inverse binomial transform of A001651.
 * https://oeis.org/A141531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1, a(2)=1; a(n)=(-2)^(n-2) for n>=3
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)1;
    return seq_pow((seq_t)-2, n - 2);
}

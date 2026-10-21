/*
 * A203427  a(n) = w(n+1)/(4*w(n)), where w = A203426.
 * https://oeis.org/A203427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/4) * (n+1) * (-2*(n+2))^n
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t n_seq = (seq_t)n;
    seq_t base = -((seq_t)2 * (n_seq + (seq_t)2));
    seq_t power = big_pow(base, n_seq);
    seq_t result = (n_seq + (seq_t)1) * power;
    result = result / (seq_t)4;
    return result;
}

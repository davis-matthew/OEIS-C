/*
 * A024084  a(n) = 7^n - n^9.
 * https://oeis.org/A024084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   big_pow, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t p7 = big_pow((seq_t)7, (seq_t)n);
    seq_t p9 = pow_seq((seq_t)n, 9);
    return p7 - p9;
}

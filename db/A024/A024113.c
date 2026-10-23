/*
 * A024113  a(n) = 9^n-n^12.
 * https://oeis.org/A024113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow9 = seq_pow((seq_t)9, n);
    seq_t pown = seq_pow((seq_t)n, (idx_t)12);
    return pow9 - pown;
}

/*
 * A024046  a(n) = 4^n - n^10.
 * https://oeis.org/A024046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4   = seq_pow((seq_t)4, n);
    seq_t pown10 = seq_pow((seq_t)n, (idx_t)10);
    return pow4 - pown10;
}

/*
 * A024034  a(n) = 3^n - n^11.
 * https://oeis.org/A024034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t pow3   = pow_int((seq_t)3, n);
    seq_t pown11 = pow_int((seq_t)n, 11);
    return pow3 - pown11;
}

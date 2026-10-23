/*
 * A171552  a(n) = 2^n*floor((5-2*n)/3).
 * https://oeis.org/A171552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^n * floor((5-2*n)/3)
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t k = (seq_t)5 - (seq_t)2 * (seq_t)n;
    seq_t floork = k / 3;
    if (k < 0 && (k % 3) != 0) {
        floork -= 1;
    }
    return pow2(n) * floork;
}

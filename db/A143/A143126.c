/*
 * A143126  a(n) = (1-2n)*2^n.
 * https://oeis.org/A143126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (1-2*n)*2^n
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 25, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow = pow2(n);
    seq_t factor = (seq_t)1 - (seq_t)2 * (seq_t)n;
    return factor * pow;
}

/*
 * A037967  a(n) = (binomial(2*n, n)^2 + binomial(2*n, n))/2.
 * https://oeis.org/A037967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (binomial(2*n, n)^2 + binomial(2*n, n)) / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t b = binomial(2 * n, n);
    seq_t tmp = b * b + b;
    return tmp / 2;
}

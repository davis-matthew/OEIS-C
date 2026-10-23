/*
 * A172362  a(n) = binomial(n+10, 10)*3^n.
 * https://oeis.org/A172362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10)*3^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t b = binomial(n + 10, 10);
    seq_t p = pow3(n);
    return b * p;
}

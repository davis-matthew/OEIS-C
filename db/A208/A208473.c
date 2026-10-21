/*
 * A208473  Central coefficients of triangle A185384.
 * https://oeis.org/A208473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n,n) * Fib(3n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t bc = binomial(2 * n, n);
    seq_t fv = fib(3 * n + 1);
    return bc * fv;
}

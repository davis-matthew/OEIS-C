/*
 * A215042  a(n) = F(8*n)/L(2*n) with n >= 0, F = A000045 (Fibonacci numbers)
 *          and L = A000032 (Lucas numbers).
 * https://oeis.org/A215042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fib(6*n) - fib(2*n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (also equals F(8n)/L(2n) for n ≥ 0) */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* multiply n inside seq_t arithmetic to avoid overflow of idx_t */
    idx_t n2 = 2*n;
    idx_t n6 = 6*n;
    seq_t f2 = fib(n2);
    seq_t f6 = fib(n6);
    return f6 - f2;
}

/*
 * A215040  a(n) = F(2*n+1)^3, n>=0, with F = A000045 (Fibonacci).
 * https://oeis.org/A215040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(2*n+1)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    idx_t m = 2 * n + 1;
    seq_t f = fib(m);
    seq_t f2 = f * f;   // f^2
    return f2 * f;       // f^3
}

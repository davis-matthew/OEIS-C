/*
 * A054888  Layer counting sequence for hyperbolic tessellation by regular
 *          pentagons of angle Pi/2.
 * https://oeis.org/A054888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Fib(2n) for n >= 1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t k = n << 1;                // k = 2*n
    return (seq_t)5 * fib(k);       // a(n) = 5 * Fib(2n)
}

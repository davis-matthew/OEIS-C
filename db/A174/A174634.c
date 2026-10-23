/*
 * A174634  a(n) = 3^n * Fibonacci(n+2).
 * https://oeis.org/A174634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * Fibonacci(n+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   fib, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t p = pow3(n);          /* 3^n   */
    seq_t f = fib(n + 2);       /* Fibonacci(n+2) */
    return p * f;               /* product */
}

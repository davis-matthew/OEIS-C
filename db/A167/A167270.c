/*
 * A167270  a(n) = Fibonacci(n + 2) + floor(n/2).
 * https://oeis.org/A167270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n + 2) + floor(n/2)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n + 2) */
    seq_t f = fib(n + 2);

    /* floor(n/2) as an unsigned integer */
    seq_t half = (seq_t)(n >> 1);

    return f + half;
}

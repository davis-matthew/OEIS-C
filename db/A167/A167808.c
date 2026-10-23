/*
 * A167808  Numerator of Fibonacci(n)/2.
 * https://oeis.org/A167808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of Fibonacci(n)/2 a(n) = (Fibonacci(n) even) ?
 *            Fibonacci(n)/2 : Fibonacci(n) with Fibonacci(0)=0,
 *            Fibonacci(1)=1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute Fibonacci(n) with 0‑based indexing using only + and assignments. */
static seq_t fib_u64(idx_t n)
{
    seq_t a = 0, b = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t t = a + b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t f = fib_u64(n);
    return (f & (seq_t)1) == 0 ? f >> 1 : f;
}

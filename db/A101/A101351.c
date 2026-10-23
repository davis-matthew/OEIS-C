/*
 * A101351  a(n) = 2^n-1 + Fibonacci(n).
 * https://oeis.org/A101351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n-1 + Fibonacci(n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   fib_fast, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    // Compute Fibonacci(n) and Fibonacci(n+1) (the latter is unused)
    seq_t fib_n, fib_next;
    fib_fast(n, &fib_n, &fib_next);

    // Compute 2^n
    seq_t pow2_n = pow2(n);

    // 2^n - 1 + Fibonacci(n)
    pow2_n -= (seq_t)1;
    pow2_n += fib_n;
    return pow2_n;
}

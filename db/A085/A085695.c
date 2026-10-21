/*
 * A085695  a(n) = Fibonacci(n)*Fibonacci(3n)/2.
 * https://oeis.org/A085695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n) * Fibonacci(3*n) / 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Return a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Fibonacci numbers for n and 3n */
    seq_t f_n  = fib(n);
    seq_t f_3n = fib(n * 3);

    /* Fibonacci(3n) is always even (since 3n is a multiple of 3),
       so we can safely divide it by 2 first to avoid overflow on
       fixed‑size types. */
    seq_t half_f_3n = f_3n >> 1;   /* equivalent to f_3n / 2 */

    /* The product now fits in seq_t when the result does. */
    return f_n * half_f_3n;
}

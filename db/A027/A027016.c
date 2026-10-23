/*
 * A027016  T(2n+1,n+1), T given by A027011.
 * https://oeis.org/A027016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A027016):
   a(0) = 1
   a(n) = Fibonacci(2n+6) – Fibonacci(2n+2) – 6n – 7   for n > 0 */
#include "../utils/fib.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* compute Fibonacci(2n+6) and Fibonacci(2n+2) */
    seq_t fib1 = fib(2*n + 6);
    seq_t fib2 = fib(2*n + 2);

    /* compute 6n + 7 */
    seq_t part = (seq_t)6 * (seq_t)n;
    part += (seq_t)7;

    /* a(n) = fib1 - fib2 - part */
    seq_t result = fib1 - fib2 - part;
    return result;
}

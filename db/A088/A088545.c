/*
 * A088545  Quotient Fibonacci(5*n)/(5*Fibonacci(n)), where Fibonacci(n) =
 *          A000045(n).
 * https://oeis.org/A088545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Fib(n)^2*(Fib(n)^2 + (-1)^n) + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* We assume n >= 1, but handle n==0 gracefully by returning 0. */
    if (n == 0) return 0;
    seq_t f   = fib(n);          /* Fibonacci(n) */
    seq_t f2  = f * f;           /* Fib(n)^2  */

    /* Compute inner term: Fib(n)^2 + (-1)^n */
    seq_t inner;
    if (n & 1) {                 /* n is odd: (-1)^n = -1 */
        inner = f2 - 1;
    } else {                     /* n is even: (-1)^n = +1 */
        inner = f2 + 1;
    }

    /* a(n) = 5 * Fib(n)^2 * inner + 1 */
    seq_t term = 5 * f2 * inner + 1;
    return term;
}

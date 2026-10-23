/*
 * A119699  a(n) = n^2*binomial(2*n, n)*Fibonacci(n)^2.
 * https://oeis.org/A119699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*binomial(2*n, n)*Fibonacci(n)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t f   = fib(n);          /* Fibonacci(n) */
    seq_t f2  = f * f;           /* Fibonacci(n)^2 */
    seq_t bin = binomial(2*n, n);
    seq_t res = f2 * bin;        /* binomial * Fibonacci(n)^2 */
    res *= (seq_t)n;             /* multiply by n */
    res *= (seq_t)n;             /* multiply by n again (n^2) */
    return res;
}

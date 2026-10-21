/*
 * A086926  Product of Fibonacci and (shifted) triangular numbers.
 * https://oeis.org/A086926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) * n*(n-1)/2
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 77, u128 n <= 166,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* F(n) is the nth Fibonacci number (F(0)=0, F(1)=1) */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Compute triangular number t = n*(n-1)/2, but avoid underflow when n < 2 */
    seq_t t;
    if (n < 2) {
        t = (seq_t)0;
    } else {
        t = ((seq_t)n * (seq_t)(n - 1)) / 2;
    }
    /* Return product of Fibonacci(n) and t */
    return fib(n) * t;
}

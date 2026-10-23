/*
 * A194158  Product of first n nonzero odd-indexed Fibonacci numbers F(1),
 *          ..., F(2*n-1).
 * https://oeis.org/A194158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{i=1..n} F(2*i-1), where F(k) is the Fibonacci number
 *            with F(1)=1,F(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) is defined to be 1 for convenience. */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the conventionally defined zero index. */
    if (n == 0) return (seq_t)1;

    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Fibonacci index is odd: 2*i-1. */
        idx_t fib_idx = 2 * i - 1;
        seq_t f = fib(fib_idx);
        prod *= f;
    }
    return prod;
}

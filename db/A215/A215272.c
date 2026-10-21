/*
 * A215272  a(n) = a(n-1)*a(n-2) with a(0)=1, a(1)=9.
 * https://oeis.org/A215272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^Fibonacci(n) (a(0) = 1, a(1) = 9)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_large.h"

/* Compute Fibonacci(n) using seq_t arithmetic.  Fib(0)=0, Fib(1)=1. */
static seq_t fib_seq(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a = (seq_t)0, b = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t t = a + b;
        a = b;
        b = t;
    }
    return b;
}

seq_t a(idx_t n)
{
    seq_t base = (seq_t)9;
    seq_t exp  = fib_seq(n);
    return pow_seq_large(base, exp);
}

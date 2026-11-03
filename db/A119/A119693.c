/*
 * A119693  a(n) = binomial(2*n,n) * Fibonacci(n)/2.
 * https://oeis.org/A119693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n,n) * fib(n) / 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    seq_t bin = binomial(2 * n, n);      /* C(2n,n) */
    seq_t fibn = fib(n);              /* Fibonacci(n) */
    seq_t prod = bin * fibn;          /* Product */
    return prod >> 1;                 /* Divide by 2 */
}

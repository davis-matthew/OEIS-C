/*
 * A094707  Partial sums of repeated Fibonacci sequence.
 * https://oeis.org/A094707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n/2 + 3) - 2 if n even, a(n) =
 *            2*Fibonacci((n-1)/2 + 2) - 2 if n odd
 * Range:     u8 n <= 20, u16 n <= 43, u32 n <= 89, u64 n <= 181,
 *            u128 n <= 366, big any n
 * Validated: standard, pass, 65 terms through n = 851 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from a(n) = Sum_{k=0..n} Fibonacci(floor(k/2)) */

#include <stdbool.h>

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
    /* n/2 is floor(n/2) */
    idx_t k = n >> 1;            /* k = floor(n/2) */
    if ((n & 1) == 0) {          /* n is even */
        seq_t fn = fib(k + 3);   /* Fibonacci(n/2 + 3) */
        return fn - (seq_t)2;
    } else {                     /* n is odd */
        seq_t fn = fib(k + 2);   /* Fibonacci((n-1)/2 + 2) */
        return fn * (seq_t)2 - (seq_t)2;
    }
}

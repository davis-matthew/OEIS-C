/*
 * A143077  a(n) is the n-th term of a pseudo-Fibonacci sequence created by
 *          applying the function fib(1,...,n) to itself n times.
 * https://oeis.org/A143077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative construction of A143077:
 * Let F(k) be the standard Fibonacci number with F(1)=F(2)=1.
 * For n≥3 compute:
 *   A = F(n-2),  B = F(n-1),  x = F(n).
 *   Repeat n-1 times:  x = A + B*x
 *   return x.
 * For n=1 or n=2, return 1. */

#include <stdint.h>
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

seq_t a(idx_t n) {
    if (n <= 2) {
        return (seq_t)1;
    }

    seq_t A = fib(n - 2);   /* F(n-2) */
    seq_t B = fib(n - 1);   /* F(n-1) */
    seq_t x = fib(n);       /* F(n) */

    for (idx_t i = 0; i < n - 1; ++i) {
        x = A + x * B;
    }
    return x;
}

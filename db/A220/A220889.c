/*
 * A220889  a(n) = F(n+8) - (1/6)*(n^4-2*n^3+26*n^2+47*n+132) where F(i) =
 *          Fibonacci numbers (A000045).
 * https://oeis.org/A220889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(n+8) - ((n⁴ - 2·n³ + 26·n² + 47·n + 132) / 6)
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 899 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fib(i) is the Fibonacci sequence with Fib(0) = 0, Fib(1) = 1. */

#include <stdbool.h>
#include <stdint.h>

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
#include <stdint.h>

seq_t a(idx_t n) {
    // Compute Fibonacci number F(n+8)
    idx_t n_plus = n + (idx_t)8;
    seq_t f = fib(n_plus);

    // Compute polynomial part in seq_t arithmetic
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;          // n²
    seq_t n3   = n2 * nn;          // n³
    seq_t n4   = n3 * nn;          // n⁴

    seq_t poly = n4 - 2 * n3 + 26 * n2 + 47 * nn + 132;  // numerator
    seq_t div  = poly / 6;                                 // exact division

    return f - div;
}

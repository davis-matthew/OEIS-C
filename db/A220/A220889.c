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
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fib(i) is the Fibonacci sequence with Fib(0) = 0, Fib(1) = 1. */

#include "../utils/fib.h"
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

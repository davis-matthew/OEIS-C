/*
 * A119698  a(n) = n^3*binomial(2*n, n)*Fibonacci(n)^2.
 * https://oeis.org/A119698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*binomial(2*n,n)*fib(n)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // Fibonacci number at index n
    seq_t fn = fib(n);
    // Square of Fib(n)
    seq_t fn2 = fn * fn;
    // n as seq_t
    seq_t nseq = (seq_t)n;
    // n^3
    seq_t n3 = nseq * nseq * nseq;
    // Binomial coefficient C(2n, n)
    seq_t bm = binomial(2 * n, n);
    // Result = n^3 * C(2n, n) * Fib(n)^2
    seq_t res = n3 * bm * fn2;
    return res;
}

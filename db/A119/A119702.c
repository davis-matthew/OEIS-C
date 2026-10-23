/*
 * A119702  a(n) = n^2*binomial(2*n, n)*Fibonacci(n).
 * https://oeis.org/A119702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*binomial(2*n, n)*Fibonacci(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t nseq = (seq_t)n;
    seq_t fib_val = fib(n);
    seq_t bin_val = binomial(2*n, n);
    seq_t res = fib_val * bin_val;
    res = res * nseq * nseq;
    return res;
}

/*
 * A119702  a(n) = n^2*binomial(2*n, n)*Fibonacci(n).
 * https://oeis.org/A119702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*binomial(2*n, n)*Fibonacci(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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
    if (n == 0) return (seq_t)0;
    seq_t nseq = (seq_t)n;
    seq_t fib_val = fib(n);
    seq_t bin_val = binomial(2*n, n);
    seq_t res = fib_val * bin_val;
    res = res * nseq * nseq;
    return res;
}

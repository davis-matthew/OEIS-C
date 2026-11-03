/*
 * A119701  a(n) = n*binomial(2*n, n)*Fibonacci(n).
 * https://oeis.org/A119701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(2*n, n)*fib(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 974 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
    if (n == 0u)
        return (seq_t)0;
    seq_t n_seq = (seq_t)n;
    seq_t fibn   = fib(n);
    seq_t bin    = binomial(2 * n, n);
    return n_seq * bin * fibn;
}

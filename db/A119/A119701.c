/*
 * A119701  a(n) = n*binomial(2*n, n)*Fibonacci(n).
 * https://oeis.org/A119701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(2*n, n)*fib(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   binom, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    seq_t n_seq = (seq_t)n;
    seq_t fibn   = fib(n);
    seq_t bin    = binom(2 * n, n);
    return n_seq * bin * fibn;
}

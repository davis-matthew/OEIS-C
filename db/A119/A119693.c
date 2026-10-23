/*
 * A119693  a(n) = binomial(2*n,n) * Fibonacci(n)/2.
 * https://oeis.org/A119693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n,n) * fib(n) / 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t bin = binom(2 * n, n);      /* C(2n,n) */
    seq_t fibn = fib(n);              /* Fibonacci(n) */
    seq_t prod = bin * fibn;          /* Product */
    return prod >> 1;                 /* Divide by 2 */
}

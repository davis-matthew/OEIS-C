/*
 * A152915  Exponacci (or exponential Fibonacci) numbers.
 * https://oeis.org/A152915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n ^ Fibonacci(n) (a(0) = 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t base = (seq_t)n;
    seq_t exp  = fib(n);
    return big_pow(base, exp);
}

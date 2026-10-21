/*
 * A209408  Number of subsets of {1,...,n} containing {a,a+4} for some a.
 * https://oeis.org/A209408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - Π_{i=0..3} Fibonacci(⌊(n+i)/4⌋+2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t total = pow2(n);
    seq_t prod  = 1;
    for (idx_t i = 0; i < 4; ++i)
    {
        idx_t k = (n + i) / 4 + 2;
        prod *= fib(k);
    }
    return total - prod;
}

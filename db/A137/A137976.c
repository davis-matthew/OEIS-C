/*
 * A137976  Fibonacci numbers (A000045) not in A103311.
 * https://oeis.org/A137976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fib((5*n + 4 - (n & 1)) / 2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the index k of the Fibonacci number to return.
       For odd n: k = (5*n + 3)/2
       For even n: k = (5*n + 4)/2   */
    idx_t k = (5 * n + 4 - (n & 1)) >> 1;
    return fib(k);
}

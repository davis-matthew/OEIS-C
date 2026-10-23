/*
 * A174883  Largest odd divisors of Fibonacci numbers.
 * https://oeis.org/A174883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd_part(F(n)) (largest odd divisor of the n-th Fibonacci
 *            number)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    /* Compute the n-th Fibonacci number and return its odd part. */
    seq_t fn = fib(n);
    return odd_part(fn);
}

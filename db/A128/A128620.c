/*
 * A128620  Row sums of A128619.
 * https://oeis.org/A128620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/2)*Fibonacci(n+1)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 85, u128 n <= 175,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t coeff = ((seq_t)n + 2) / 2;   /* floor((n+2)/2) */
    seq_t fn, fn1;
    fib_pair(n + 1, &fn, &fn1);         /* fn = Fibonacci(n+1) */
    return coeff * fn;
}

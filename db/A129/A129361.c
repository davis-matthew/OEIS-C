/*
 * A129361  a(n) = Sum_{k=floor((n+1)/2)..n} Fibonacci(k+1).
 * https://oeis.org/A129361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(n+3) - Fib(floor((n+1)/2)+2)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    idx_t n_plus_3 = n + 3;          /* n+3  */
    idx_t half     = (n + 1) / 2;    /* floor((n+1)/2) */
    idx_t start_idx = half + 2;      /* floor((n+1)/2)+2 */

    seq_t high = fib(n_plus_3);
    seq_t low  = fib(start_idx);

    return high - low;
}

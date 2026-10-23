/*
 * A188516  Number of nX2 binary arrays without the pattern 1 1 0 diagonally,
 *          vertically or horizontally.
 * https://oeis.org/A188516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fibonacci(n+3) - 1)^2 where Fibonacci is the standard
 *            Fibonacci sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n+3). */
    seq_t fn = fib(n + 3);
    /* Subtract one. */
    seq_t f  = fn - 1;
    /* Return square. */
    return f * f;
}

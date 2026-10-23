/*
 * A131780  Row sums of triangle A131779.
 * https://oeis.org/A131780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(n+1) - (1 - (-1)^n)/2 = 2*Fibonacci(n+1) - (n
 *            % 2)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci indexes start at 0 with F(0)=0, F(1)=1.  We need F(n+1). */
    seq_t val = fib(n + 1);
    val = val * 2ULL;                 /* multiply by 2 */
    if (n & 1)                         /* subtract 1 if n is odd */
        val -= (seq_t)1;
    return val;
}

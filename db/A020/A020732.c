/*
 * A020732  Pisot sequence T(4,7).
 * https://oeis.org/A020732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+5) - 1
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function fib in ../utils/fib.h which
   returns the (n)th Fibonacci number with F(0)=0, F(1)=1.   */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t v = fib(n + 5);
    v -= 1;
    return v;
}

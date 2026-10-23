/*
 * A133585  Expansion of x - x^2*(2*x+1)*(x^2-2) / ( (x^2-x-1)*(x^2+x-1) ).
 * https://oeis.org/A133585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1) = 1 * a(n) = F(n+1) if n is even * a(n) = 2 * F(n) if n
 *            is odd and n > 1 * where F(k) is the Fibonacci number with
 *            F(0)=0, F(1)=1. * * The helper function fib(idx_t n) defined in
 *            ../utils/fib.h * returns F(n)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the Fibonacci helper used by the build\'s utilities */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Offset is 1, so n >= 1 for valid calls. */
    if (n == 1)
        return (seq_t)1;
    if ((n & 1) == 0)          /* even n */
        return fib(n + 1);
    else                       /* odd n > 1 */
        return (seq_t)2 * fib(n);
}

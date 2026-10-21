/*
 * A240836  Numbers n such that n^3 = x*y*z where 2 <= x <= y <= z , n^3+1 =
 *          (x-1)*(y+1)*(z+1).
 * https://oeis.org/A240836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * F(2n) * F(2n-1) (F = Fibonacci numbers)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci numbers are 0‑based: F(0)=0, F(1)=1 */
    seq_t f2n   = fib(2 * n);
    seq_t f2n_1 = fib(2 * n - 1);
    return 2 * f2n * f2n_1;
}

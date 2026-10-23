/*
 * A115852  a(n) = ceiling((F(n)^4-1)^2 / (4*F(n)^4)) where F(n) = A000045(n)
 *          are the Fibonacci numbers.
 * https://oeis.org/A115852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (F(n)^4 + 2) / 4, where F(n) is the n-th Fibonacci number
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f  = fib(n);          /* F(n) */
    seq_t f2 = f * f;           /* F(n)^2  */
    seq_t f4 = f2 * f2;         /* F(n)^4  */
    return (f4 + (seq_t)2) / (seq_t)4;
}

/*
 * A237268  a(1)=1; for n > 1, a(n) is the smallest F(m) > F(n) such that F(n)
 *          divides F(m), where F(k) denotes the k-th Fibonacci number.
 * https://oeis.org/A237268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) equals the Fibonacci number F(2n) for n >= 1,
   except a(2) = 2 (because F(4) = 3 is replaced by 2). */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)2;
    return fib(2 * n);
}

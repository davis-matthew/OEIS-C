/*
 * A122629  n_F(n) where F() = Fibonacci numbers A000045.
 * https://oeis.org/A122629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n/10)*F(n) + n mod 10 where F(n) is the nth Fibonacci
 *            number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    idx_t q = n / 10;           /* integer quotient n/10 */
    seq_t k = (seq_t)q;          /* cast to seq_t for multiplication */
    seq_t f = fib(n);           /* nth Fibonacci number */
    return k * f + (seq_t)(n % 10);
}

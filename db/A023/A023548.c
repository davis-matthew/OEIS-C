/*
 * A023548  Convolution of natural numbers >= 2 and Fibonacci numbers.
 * https://oeis.org/A023548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+4) + F(n+2) - (n+4) where F(k) denotes the k-th
 *            Fibonacci number (F(0)=0, F(1)=1). This follows from the OEIS
 *            entry that a(n)=L(n+3)-(n+4) or the equivalent convolution
 *            formula. The function uses the provided fib() routine to obtain
 *            Fibonacci numbers
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 (offset 1) */
    /* compute Fibonacci numbers at n+2 and n+4 */
    seq_t fn2 = fib(n + 2);
    seq_t fn4 = fib(n + 4);

    /* sum of the two Fibonacci numbers */
    seq_t sum = fn2 + fn4;

    /* subtract (n+4) */
    seq_t sub = (seq_t)(n + 4);

    return sum - sub;
}

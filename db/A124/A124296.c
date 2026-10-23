/*
 * A124296  a(n) = 5*F(n)^2 - 5*F(n) + 1, where F(n) = Fibonacci(n).
 * https://oeis.org/A124296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Fibonacci(n)^2 - 5*Fibonacci(n) + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n);          /* Fibonacci(n) */
    seq_t res = f * f;         /* f^2 */
    res = res * 5;             /* 5*f^2 */
    seq_t f5 = f * 5;          /* 5*f */
    res = res - f5 + 1;        /* 5*f^2 - 5*f + 1 */
    return res;
}

/*
 * A080143  a(n) = F(3)*F(n)*F(n+1) + F(4)*F(n+1)^2 - F(4) if n even,
 *          F(3)*F(n)*F(n+1) + F(4)*F(n+1)^2 if n odd, where F(n) is the n-th
 *          Fibonacci number (A000045).
 * https://oeis.org/A080143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*Fibonacci(n+2)*Fibonacci(n+3) - 3 - (-1)^n)/2
 *            Equivalent: a(n) = Fibonacci(n+2)*Fibonacci(n+3) - {2 if n even
 *            else 1}
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute F(n+2) and F(n+3) */
    seq_t f_n2 = fib(n + 2);
    seq_t f_n3 = fib(n + 3);

    /* Product of consecutive Fibonacci numbers */
    seq_t prod = f_n2 * f_n3;

    /* Subtract 2 for even n, 1 for odd n */
    seq_t sub_half = (n & 1ULL) ? 1 : 2;

    return prod - sub_half;
}

/*
 * A202462  a(n) = Sum_{j=1..n} Sum_{i=1..n} F(i,j), where F is the Fibonacci
 *          fusion array of A202453.
 * https://oeis.org/A202462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+2)*Fibonacci(n+3) - 2*Fibonacci(n+4) + n + 4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the necessary Fibonacci values */
    seq_t f2 = fib(n + 2);
    seq_t f3 = fib(n + 3);
    seq_t f4 = fib(n + 4);

    /* Apply the formula */
    seq_t result = f2 * f3;          // F(n+2) * F(n+3)
    result -= f4 * 2;                // subtract 2*F(n+4)
    result += (seq_t)(n + 4);        // add n+4

    return result;
}

/*
 * A220362  a(n) = Fibonacci(n-1) * Fibonacci(n) * Fibonacci(n+2).
 * https://oeis.org/A220362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n-1) * Fibonacci(n) * Fibonacci(n+2) (offset =
 *            1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the underflow that occurs when computing fib(n-1) for n = 1 */
    if (n == 1) {
        /* fib(0) * fib(1) * fib(3) = 0 * 1 * 2 = 0 */
        return 0;
    }
    seq_t f_prev = fib(n - 1);   /* Fibonacci(n-1) */
    seq_t f_curr = fib(n);       /* Fibonacci(n)   */
    seq_t f_next = fib(n + 2);   /* Fibonacci(n+2) */
    return f_prev * f_curr * f_next;
}

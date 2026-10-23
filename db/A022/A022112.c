/*
 * A022112  Fibonacci sequence beginning 2, 6.
 * https://oeis.org/A022112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*Fibonacci(n+2) - 2*Fibonacci(n+1)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* compute Fibonacci(n+1) and Fibonacci(n+2) using the utility function */
    seq_t f_prev = fib(n + 1);   /* F(n+1) */
    seq_t f_curr = fib(n + 2);   /* F(n+2) */

    return (seq_t)4 * f_curr - (seq_t)2 * f_prev;
}

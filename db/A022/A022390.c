/*
 * A022390  Fibonacci sequence beginning 8, 17.
 * https://oeis.org/A022390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*Fibonacci(n+2) + Fibonacci(n)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    /* fib_pair returns fib(n) in fn and fib(n+1) in fn1 */
    fib_pair(n, &fn, &fn1);
    seq_t fn2 = fn1 + fn;              /* fib(n+2) = fib(n+1) + fib(n) */
    seq_t res = fn2 * (seq_t)8 + fn;   /* a(n) = 8*fn2 + fn */
    return res;
}

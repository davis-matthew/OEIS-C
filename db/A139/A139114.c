/*
 * A139114  Concatenation of n-th Fibonacci number and n.
 * https://oeis.org/A139114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(Fibonacci(n), n)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 171,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci number F(n) */
    seq_t fn = fib(n);

    /* number of decimal digits of n */
    int d = 0;
    idx_t t = n;
    do { d++; t /= 10; } while (t);

    /* 10^d as a seq_t */
    seq_t pow = 1;
    for (int i = 0; i < d; i++)
        pow *= 10;

    /* concatenate: F(n) * 10^d + n */
    return fn * pow + (seq_t)n;
}

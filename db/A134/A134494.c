/*
 * A134494  a(n) = Fibonacci(6n+2).
 * https://oeis.org/A134494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(6*n+2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling algorithm for Fibonacci numbers.
   Returns F(n) in *f0 and F(n+1) in *f1. */
static void fib_pair(idx_t n, seq_t *f0, seq_t *f1)
{
    if (n == 0) {
        *f0 = 0;
        *f1 = 1;
        return;
    }
    fib_pair(n >> 1, f0, f1);
    seq_t a = *f0;
    seq_t b = *f1;
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);          /* F(2k)   */
    seq_t d = (a * a) + (b * b);        /* F(2k+1) */
    if ((n & 1) == 0) {
        *f0 = c;
        *f1 = d;
    } else {
        *f0 = d;
        *f1 = c + d;
    }
}

seq_t a(idx_t n)
{
    idx_t m = 6 * n + 2;                 /* index for Fibonacci */
    seq_t fn;
    seq_t dummy;                         /* not used */
    fib_pair(m, &fn, &dummy);
    return fn;
}

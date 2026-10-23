/*
 * A135994  First differences of A135992.
 * https://oeis.org/A135994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (Lucas(n) - (-1)^n * Fibonacci(n+3)) / 2 Lucas(n) = 2 *
 *            Fibonacci(n+1) - Fibonacci(n) for all n >= 0 (works also for n =
 *            0: Lucas(0) = 2 * F(1) - F(0) = 2)
 * Range:     i8 n <= 8, i16 n <= 20, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci values needed */
    seq_t fn   = fib(n);     /* F(n)   */
    seq_t fn1  = fib(n+1);   /* F(n+1) */
    seq_t fnp3 = fib(n+3);   /* F(n+3) */

    /* Lucas number: L(n) = 2*F(n+1) - F(n) */
    seq_t lucas = fn1 * (seq_t)2 - fn;

    /* Sign adjustment for (-1)^n */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;

    /* a(n) = (L(n) - sign * F(n+3)) / 2 */
    return (lucas - sign * fnp3) / (seq_t)2;
}

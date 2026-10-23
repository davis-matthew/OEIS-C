/*
 * A137392  (10-n) * Fibonacci(n).
 * https://oeis.org/A137392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (10 - n) * Fibonacci(n) where Fibonacci(1)=1,
 *            Fibonacci(2)=1 (i.e. standard Fibonacci with F0=0)
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling implementation of Fibonacci.
   fib_pair(n) returns F(n) in *fn and F(n+1) in *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);              // a = F(k), b = F(k+1), k = n/2
    seq_t two_b_minus_a = (b << 1) - a;    // 2*F(k+1) - F(k)
    seq_t c = a * two_b_minus_a;           // F(2k) = F(k) * [2*F(k+1)-F(k)]
    seq_t d = a * a + b * b;               // F(2k+1) = F(k)^2 + F(k+1)^2
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset starts at 1; n=0 not defined
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);        // compute Fibonacci(n)
    seq_t factor = (seq_t)10 - (seq_t)n;
    return factor * fn;
}

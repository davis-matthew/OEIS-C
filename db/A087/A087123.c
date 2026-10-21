/*
 * A087123  a(n) = Fibonacci(n+1) - (-1)^n*Fibonacci(n).
 * https://oeis.org/A087123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+1) - (-1)^n * Fibonacci(n)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Fast‑doubling Fibonacci: returns F(n) and F(n+1) in *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);          /* a=F(k), b=F(k+1) where k=n/2 */

    /* c = F(2k) = F(k) * (2*F(k+1) – F(k)) */
    seq_t two_b = b + b;
    seq_t t     = two_b - a;           /* 2*F(k+1) – F(k) is non‑negative */
    c = a * t;

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    d = a * a + b * b;

    if (n & 1)                         /* n is odd: F(2k+1), F(2k+2) */
    {
        *fn  = d;
        *fn1 = c + d;
    }
    else                               /* n is even: F(2k), F(2k+1) */
    {
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);            /* fn  = F(n)   , fn1 = F(n+1) */

    if ((n & 1) == 0)                  /* n even: subtract */
        return fn1 - fn;
    else                               /* n odd : add    */
        return fn1 + fn;
}

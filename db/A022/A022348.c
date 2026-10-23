/*
 * A022348  Fibonacci sequence beginning 0, 14.
 * https://oeis.org/A022348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14 * fib(n), where fib(0)=0, fib(1)=1,
 *            fib(n)=fib(n-1)+fib(n-2)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive fast‑doubling algorithm that returns F(n) in *fn
   and F(n+1) in *fn1.  Does not use any other external helper. */
static void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    /* compute pair for floor(n/2) */
    fib_pair_rec(n >> 1, fn, fn1);
    seq_t a = *fn;
    seq_t b = *fn1;

    /* c = a * (2*b - a)
       d = a*a + b*b */
    seq_t t1 = b << 1; /* 2*b */
    seq_t c = a * (t1 - a);
    seq_t d = a * a + b * b;

    if (n & 1) {     /* odd n: F(n) = d, F(n+1) = c + d */
        *fn  = d;
        *fn1 = c + d;
    } else {        /* even n: F(n) = c, F(n+1) = d */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair_rec(n, &fn, &fn1);   /* fn = F(n) */
    return fn * (seq_t)14;        /* a(n) = 14*F(n) */
}

/*
 * A195547  Denominators a(n) of Pythagorean approximations b(n)/a(n) to 1/2.
 * https://oeis.org/A195547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2*F(n)*F(n+1)/(2-((n+2)^2 mod 3))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F(0)=0, F(1)=1 are the Fibonacci numbers.       */
/* The function computes the n-th term for n ≥ 1 using the      */
/* fast–doubling Fibonacci algorithm and rational arithmetic. */

#include <stdint.h>

/* Fast doubling: compute Fn and Fn+1.  Handles all integer and            */
/* arbitrarily large seq_t values because only +, -, * operations are     */
/* used (operator overloads are provided for seq_t). */
static void fib_pair(idx_t n, seq_t *Fn, seq_t *Fn1)
{
    if (n == 0) {
        *Fn  = (seq_t)0;
        *Fn1 = (seq_t)1;
        return;
    }
    seq_t a, b, c, d, t;
    fib_pair(n >> 1, &a, &b);

    t = b * (seq_t)2 - a;          /* F(2k) = a*(2*b - a)    */
    c = a * t;                     /* F(2k)                  */
    d = a * a + b * b;             /* F(2k+1) */

    if (n & 1) {
        *Fn  = d;                  /* F(2k+1) = Fn           */
        *Fn1 = c + d;              /* F(2k+2) = Fn1          */
    } else {
        *Fn  = c;                  /* F(2k)   = Fn           */
        *Fn1 = d;                  /* F(2k+1) = Fn1          */
    }
}

seq_t a(idx_t n)
{
    seq_t Fn, Fn1;
    fib_pair(n, &Fn, &Fn1);

    /* denom = 2 - ((n+2)^2 mod 3).  It is 1 or 2. */
    idx_t t     = n + 2;
    idx_t denom = 2 - ((t * t) % 3);

    seq_t prod = Fn * Fn1;          /* F(n)*F(n+1)             */

    if (denom == 1)                 /* multiply by 2 if denom==1 */
        prod = prod * (seq_t)2;

    return prod;                    /* this is a(n)            */
}

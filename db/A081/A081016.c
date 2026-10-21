/*
 * A081016  a(n) = (Lucas(4*n+3) + 1)/5, or Fibonacci(2*n+1)*Fibonacci(2*n+2),
 *          or A081015(n)/5.
 * https://oeis.org/A081016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+1) * Fibonacci(2*n+2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast doubling Fibonacci algorithm. */
#include <stdint.h>

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    /* Returns F(n) and F(n+1) by fast doubling. */
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t f0, f1;
    fib_pair(n >> 1, &f0, &f1);           /* f0 = F(k), f1 = F(k+1), k = n/2 */
    seq_t a = f0;                          /* F(k)   */
    seq_t b = f1;                          /* F(k+1) */
    seq_t two_b = b + b;                   /* 2*F(k+1) */
    seq_t c = a * (two_b - a);             /* F(2k)   */
    seq_t d = a * a + b * b;               /* F(2k+1) */
    if (n & 1) {
        *fn  = d;                          /* F(2k+1) */
        *fn1 = c + d;                      /* F(2k+2) */
    } else {
        *fn  = c;                          /* F(2k)   */
        *fn1 = d;                          /* F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;                   /* compute argument 2*n+1 */
    seq_t f1, f2;                          /* f1 = F(2n+1), f2 = F(2n+2) */
    fib_pair(m, &f1, &f2);
    return f1 * f2;                        /* a(n) = F(2n+1)*F(2n+2) */
}

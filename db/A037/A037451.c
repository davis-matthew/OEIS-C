/*
 * A037451  a(n) = Fibonacci(n) * Fibonacci(2*n).
 * https://oeis.org/A037451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) * Fibonacci(2*n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, but seq_t/idx_t are defined by the build */

static void fib_pair_local(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_local(n >> 1, &a, &b);           /* a = F(k), b = F(k+1) where k = floor(n/2) */

    /* F(2k) = F(k) * (2*F(k+1) - F(k)) */
    seq_t two_b = b + b;                 /* 2*F(k+1) */
    seq_t c = a * (two_b - a);

    /* F(2k+1) = F(k+1)^2 + F(k)^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {              /* even n:  n = 2k  */
        *fn  = c;
        *fn1 = d;
    } else {                          /* odd n:   n = 2k+1*/
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair_local(n, &fn, &fn1);          /* fn  = F(n),   fn1 = F(n+1) */

    /* Compute F(2n) = F(n) * (2*F(n+1) - F(n)) */
    seq_t two_fn1 = fn1 + fn1;
    seq_t inner   = two_fn1 - fn;
    seq_t f2n     = fn * inner;

    return fn * f2n;                       /* F(n) * F(2n) */
}

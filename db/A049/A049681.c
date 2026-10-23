/*
 * A049681  a(n) = (Lucas(2*n) - Lucas(n))/2.
 * https://oeis.org/A049681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Lucas(2*n) - Lucas(n))/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci pair: fn = F(n), fn1 = F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {               /* F(0)=0, F(1)=1 */
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;                 /* a=F(k), b=F(k+1) for k = n/2 */
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k)   = a*(2*b - a) */
    seq_t two_b = b << 1;
    seq_t c = a * (two_b - a);

    /* d = F(2k+1) = a*a + b*b */
    seq_t d = a * a + b * b;

    if (n & 1) {                /* n is odd: n = 2k+1 */
        *fn  = d;               /* F(2k+1) */
        *fn1 = c + d;           /* F(2k+2) = F(n+1) */
    } else {                    /* n is even: n = 2k */
        *fn  = c;               /* F(2k)   */
        *fn1 = d;               /* F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1, f2n, f2n1;

    /* Fibonacci numbers for n and 2*n */
    fib_pair(n, &fn, &fn1);          /* fn  = F(n),   fn1 = F(n+1)   */
    fib_pair(2 * n, &f2n, &f2n1);    /* f2n = F(2n),  f2n1 = F(2n+1) */

    /* Lucas numbers: L(k) = 2*F(k+1) - F(k) */
    seq_t lucas_n   = (fn1 << 1) - fn;   /* L(n)   */
    seq_t lucas_2n  = (f2n1 << 1) - f2n; /* L(2n)  */

    return (lucas_2n - lucas_n) / ((seq_t)2);
}

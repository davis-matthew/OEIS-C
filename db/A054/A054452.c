/*
 * A054452  Partial sums of A027941(n-1) with a(-1) = 0.
 * https://oeis.org/A054452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fib(2*n) - n, with Fibonacci F(0)=0, F(1)=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling: computes F(n) and F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = b + b;                /* 2*F(k+1) */
    seq_t c = a * (two_b - a);          /* F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    seq_t d = a * a + b * b;           /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if (n & 1) {
        *fn  = d;                       /* F(2k+1) */
        *fn1 = c + d;                   /* F(2k+2) = F(2k) + F(2k+1) */
    } else {
        *fn  = c;                       /* F(2k)   */
        *fn1 = d;                       /* F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(2 * n, &fn, &fn1);        /* fn = F(2n) */
    return fn - (seq_t)n;              /* a(n) = F(2n) - n */
}

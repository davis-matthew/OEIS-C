/*
 * A027979  a(n) = Sum_{k=0..n} T(n,k)*T(n,2n-k), T given by A027960.
 * https://oeis.org/A027979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*Lucas(2*n) + 3*Fibonacci(2*n) - (-1)^n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Fast doubling method: compute (F(n), F(n+1)) into fn, fn1. */
static void
fib_pair_local(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_local(n >> 1, &a, &b);           /* a = F(k), b = F(k+1), k = ⌊n/2⌋ */

    seq_t c = a * ((b << 1) - a);             /* F(2k)  = a * (2*b - a)  */
    seq_t d = a * a + b * b;                  /* F(2k+1)= a*a + b*b  */

    if ((n & 1) == 0) {                       /* n = 2k   */
        *fn  = c;
        *fn1 = d;
    } else {                                  /* n = 2k+1 */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t
a(idx_t n)
{
    seq_t f2n, f2n1;            /* Fibonacci values for 2n and 2n+1 */
    seq_t lucas, term;

    /* Compute F(2n) and F(2n+1) */
    fib_pair_local((idx_t)(2 * n), &f2n, &f2n1);

    /* Lucas(2n) = 2*F(2n+1) - F(2n) */
    lucas = f2n1 * (seq_t)2;
    lucas -= f2n;

    /* term = (n+1)*Lucas(2n) + 3*Fibonacci(2n) */
    term = lucas * (seq_t)(n + 1);
    term += f2n * (seq_t)3;

    /* subtract (-1)^n */
    if ((n & 1) == 0)                 /* n even: (-1)^n = 1  */
        term -= 1;
    else                              /* n odd : (-1)^n = -1 */
        term += 1;

    return term;
}

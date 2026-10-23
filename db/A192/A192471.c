/*
 * A192471  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=1+x^n+x^(2n+1).
 * https://oeis.org/A192471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + F_{n-1} + F_{2n} (F_k: Fibonacci numbers,
 *            F_0=0,F_1=1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F_k,   b = F_{k+1} */
    seq_t two_b = b + b;                /* 2*F_{k+1}   */
    seq_t c = a * (two_b - a);          /* F_{2k}      */
    seq_t d = a * a + b * b;            /* F_{2k+1}    */
    if ((n & 1) == 0) {                 /* n even      */
        *fn  = c;
        *fn1 = d;
    } else {                            /* n odd       */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* not part of OEIS offset */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);             /* F_n , F_{n+1}   */
    seq_t fn_minus1 = fn1 - fn;         /* F_{n-1}         */
    seq_t two_b = fn1 + fn1;            /* 2*F_{n+1}       */
    seq_t fn2n = fn * (two_b - fn);     /* F_{2n}          */
    return (seq_t)1 + fn_minus1 + fn2n; /* constant term   */
}

/*
 * A020956  a(n) = Sum_{k>=1} floor(tau^(n-k)) where tau is A001622.
 * https://oeis.org/A020956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n+1) – floor((n+1)/2) – 1 Lucas numbers: L(0)=2,
 *            L(1)=1, L(n)=L(n-1)+L(n-2) Lucas(n+1) = 2*F(n+2) – F(n+1) where
 *            F is Fibonacci (F(0)=0,F(1)=1) The implementation uses fast
 *            doubling to compute F(k) and F(k+1) in O(log k) time
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast doubling: computes Fibonacci pair (F(n), F(n+1)) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);            /* a = F(k), b = F(k+1) */
    seq_t two_b = b + b;                 /* 2*F(k+1) */
    seq_t tmp   = two_b - a;             /* 2*F(k+1) – F(k) */
    seq_t c = a * tmp;                   /* F(2k) */
    seq_t d = a * a + b * b;             /* F(2k+1) */
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
    /* Compute Lucas(n+1) = 2*F(n+2) – F(n+1) */
    seq_t fn, fn1;                       /* fn = F(n+1), fn1 = F(n+2) */
    fib_pair(n + 1, &fn, &fn1);
    seq_t lucas = (fn1 + fn1) - fn;      /* 2*F(n+2) – F(n+1) */

    /* Subtract floor((n+1)/2) + 1 */
    idx_t half = (n + 1) >> 1;           /* floor((n+1)/2) */
    seq_t sub = (seq_t)half + 1;

    return lucas - sub;
}

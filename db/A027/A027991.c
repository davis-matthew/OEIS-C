/*
 * A027991  a(n) = Sum{T(n,k)*T(n,2n-k)}, 0<=k<=n-1, T given by A027926.
 * https://oeis.org/A027991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5)[ n*F(2n+2) - n*F(2n-2) + F(2n-1) - (-1)^n ]
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci F(0)=0, F(1)=1 (fast doubling, works for all idx_t) */
#include <stdint.h>

static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* (F(k), F(k+1))  */
    seq_t two_b = b + b;                /* 2*F(k+1)          */
    seq_t c = a * (two_b - a);          /* F(2k)              */
    seq_t d = a * a + b * b;            /* F(2k+1)            */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

static inline seq_t fib(idx_t n)
{
    seq_t f, _;
    fib_pair(n, &f, &_);
    return f;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not part of the sequence, safe guard */

    seq_t f2n2  = fib(2 * n + 2);       /* F(2n+2)          */
    seq_t f2n1  = fib(2 * n - 1);       /* F(2n-1)          */
    seq_t f2n2m = fib(2 * n - 2);       /* F(2n-2)          */

    seq_t t = (seq_t)n * f2n2;          /* n*F(2n+2)        */
    t -= (seq_t)n * f2n2m;              /* - n*F(2n-2)      */
    t += f2n1;                           /* + F(2n-1)        */

    /* subtract (-1)^n :
       n even -> subtract 1
       n odd  -> add 1                                      */
    if ((n & 1) == 0)
        t -= (seq_t)1;
    else
        t += (seq_t)1;

    return t / (seq_t)5;               /* division is exact */
}

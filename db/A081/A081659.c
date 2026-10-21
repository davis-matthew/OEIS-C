/*
 * A081659  a(n) = n + Fibonacci(n+1).
 * https://oeis.org/A081659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + F(n+1) (offset 0, F(0)=0, F(1)=1)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci: given n, set *fn = F(n), *fn1 = F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;              /* a = F(k), b = F(k+1) where k = n/2 */
    fib_pair(n >> 1, &a, &b);

    seq_t two_b   = b + b;                           /* 2*F(k+1)            */
    seq_t c       = a * (two_b - a);                /* F(2k)                */
    seq_t d       = a * a + b * b;                  /* F(2k+1)              */

    if ((n & 1) == 0) {         /* n even: return F(2k), F(2k+1) */
        *fn  = c;
        *fn1 = d;
    } else {                    /* n odd : return F(2k+1), F(2k+2) */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;                 /* fn  = F(n),   fn1 = F(n+1) */
    fib_pair(n, &fn, &fn1);
    return fn1 + (seq_t)n;         /* a(n) = n + F(n+1)            */
}

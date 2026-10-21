/*
 * A099014  a(n) = Fibonacci(n)*(Fibonacci(n-1)^2 + Fibonacci(n+1)^2).
 * https://oeis.org/A099014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) * (Fibonacci(n-1)^2 + Fibonacci(n+1)^2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast‑doubling algorithm for Fibonacci numbers
   fib_pair(k) returns (F_k , F_{k+1}) in *f and *g   */
static void fib_pair(idx_t k, seq_t *f, seq_t *g)
{
    if (k == 0) {
        *f = 0;
        *g = 1;
        return;
    }
    seq_t a, b;
    fib_pair(k >> 1, &a, &b);          /* F_{k/2} , F_{k/2+1} */

    /* 2*F_{k/2+1} - F_{k/2} */
    seq_t t = b;
    t *= 2;
    t -= a;

    seq_t c = a * t;                    /* F_{2*(k/2)} = F_k if k even   */
    seq_t d = a * a + b * b;            /* F_{2*(k/2)+1}               */

    if ((k & 1) == 0) {                /* k even */
        *f = c;
        *g = d;
    } else {                           /* k odd  */
        *f = d;                        /* F_{k} = F_{2*(k/2)+1} */
        *g = c + d;                    /* F_{k+1} = F_{k} + F_{k-1}  */
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);            /* F_n, F_{n+1} */

    /* F_{n-1} = F_{n+1} - F_n */
    seq_t fm1 = fn1 - fn;

    /* (F_{n-1})^2 + (F_{n+1})^2 */
    seq_t term = fm1 * fm1 + fn1 * fn1;

    return fn * term;
}

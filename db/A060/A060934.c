/*
 * A060934  Second column of Lucas bisection triangle (even part).
 * https://oeis.org/A060934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*Lucas(2*n+2) + Fibonacci(2*n+2) (Lucas and Fibonacci
 *            numbers are generated via fast doubling.)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Fast doubling Fibonacci: returns (F(n), F(n+1)) in *fn and *fn1   */
/* ------------------------------------------------------------------ */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);              /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b   = b + b;                  /* 2*F(k+1) */
    seq_t c       = a * (two_b - a);        /* F(2k) = F(k) * [2F(k+1) – F(k)] */
    seq_t d       = a * a + b * b;          /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {                     /* even n */
        *fn  = c;
        *fn1 = d;
    } else {                                /* odd n */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* ------------------------------------------------------------------ */
/* The required sequence function                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* m = 2*n + 2 */
    idx_t m = (idx_t)2 * n + 2;

    seq_t f_m, f_m1;
    fib_pair(m, &f_m, &f_m1);              /* f_m = F(m), f_m1 = F(m+1) */

    /* Lucas number L(m) = 2*F(m+1) - F(m) */
    seq_t L_m = f_m1 + f_m1 - f_m;

    /* a(n) = 2*n*L_m + F(m) */
    seq_t two_n = (seq_t)2 * (seq_t)n;     /* cast to seq_t for multiplication */
    seq_t tmp   = two_n * L_m;
    seq_t res   = tmp + f_m;

    return res;
}

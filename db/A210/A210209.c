/*
 * A210209  GCD of all sums of n consecutive Fibonacci numbers.
 * https://oeis.org/A210209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * 0 if n == 0 * 1 if n is odd and n mod 3 != 0 * 2 if n
 *            is odd and n mod 3 == 0 * F(n/2) if n is even and n mod 4 == 0 *
 *            L(n/2) if n is even and n mod 4 == 2 * * F(k) = Fibonacci(k) *
 *            L(k) = Lucas(k) = 2*F(k+1) - F(k) * * This implementation uses
 *            the fast‑doubling method to compute Fibonac...
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 93, u64 n <= 185,
 *            u128 n <= 369, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast‑doubling algorithm for Fibonacci numbers.
 * Produces (F(n), F(n+1)) in *fn and *fn1.
 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;                   /* (F(k), F(k+1)) for k = n >> 1 */
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k)   = F(k) * [2*F(k+1) – F(k)]      */
    /* d = F(2k+1) = F(k)^2 + F(k+1)^2               */
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);
    seq_t d = a * a + b * b;

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
    if (n == 0) {
        return (seq_t)0;
    }

    /* Odd n */
    if (n & 1) {
        return (n % 3 == 0) ? (seq_t)2 : (seq_t)1;
    }

    /* Even n */
    idx_t k = n >> 1;             /* n / 2 */

    /* If n % 4 == 0 → output Fibonacci(k) */
    if ((k & 1) == 0) { /* n mod 4 == 0 */
        seq_t fk, fk1;
        fib_pair(k, &fk, &fk1);
        return fk;                /* F(k) */
    }

    /* n % 4 == 2 → output Lucas(k) = 2*F(k+1) – F(k) */
    seq_t fk, fk1;
    fib_pair(k, &fk, &fk1);      /* fk = F(k), fk1 = F(k+1) */
    return (seq_t)2 * fk1 - fk;  /* Lucas(k) */
}

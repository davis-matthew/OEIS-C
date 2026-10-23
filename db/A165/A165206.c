/*
 * A165206  a(n) = (3-4*n)*F(2*n-2) + (4-7*n)*F(2*n-1).
 * https://oeis.org/A165206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (3-4*n)*F(2*n-2) + (4-7*n)*F(2*n-1)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 18, i64 n <= 40, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast‑doubling Fibonacci to compute F(k) for any integer k.  */

#include <stdint.h>

/* Fast doubling: fib_pair(n) returns F(n) in *fn and F(n+1) in *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {          /* base case */
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    fib_pair(n >> 1, fn, fn1);    /* recurse on n/2 */
    seq_t a = *fn;                /* F(k)   */
    seq_t b = *fn1;               /* F(k+1) */

    /* F(2k)   = F(k) * [2*F(k+1) – F(k)] */
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);

    /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {          /* n even */
        *fn  = c;
        *fn1 = d;
    } else {                     /* n odd  */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Compute the nth term of OEIS sequence A165206.  */
seq_t a(idx_t n)
{
    /* Indices for the needed Fibonacci numbers: 2*n-2 and 2*n-1.  */
    int64_t k1 = (int64_t)2 * (int64_t)n - 2;   /* may be negative */
    int64_t k2 = (int64_t)2 * (int64_t)n - 1;   /* may be -1 for n=0 */

    seq_t f1, f2;
    seq_t dummy1, dummy2;   /* placeholders for unused return value */

    if (k1 >= 0) {
        fib_pair((idx_t)k1, &f1, &dummy1);
    } else {
        /* Use Fibonacci identity for negative indices:
           F(-m) = (-1)^{m+1} * F(m)   (m > 0) */
        idx_t m = (idx_t)(-k1);
        seq_t tmp;
        fib_pair(m, &tmp, &dummy1);
        f1 = ((m & 1) ? 1 : -1) * tmp;
    }

    if (k2 >= 0) {
        fib_pair((idx_t)k2, &f2, &dummy2);
    } else {
        idx_t m = (idx_t)(-k2);
        seq_t tmp;
        fib_pair(m, &tmp, &dummy2);
        f2 = ((m & 1) ? 1 : -1) * tmp;
    }

    /* Coefficients (3-4*n) and (4-7*n). */
    seq_t coeff1 = (seq_t)3 - (seq_t)4 * (seq_t)n;
    seq_t coeff2 = (seq_t)4 - (seq_t)7 * (seq_t)n;

    return coeff1 * f1 + coeff2 * f2;
}

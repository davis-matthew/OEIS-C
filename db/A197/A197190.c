/*
 * A197190  a(0) = 2, a(n) = Lucas(phi(n^2)) for n > 0.
 * https://oeis.org/A197190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lucas(phi(n^2)) for n>0, with a(0) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas number L(k) = 2 * F(k+1) – F(k) where F are Fibonacci numbers */

/* Helper: fast doubling algorithm for Fibonacci pairs.
   Computes F(n) and F(n+1) returning them via pointers fn and fn1.   */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;                     /* a = F(n/2),  b = F(n/2+1) */
    fib_pair(n >> 1, &a, &b);

    seq_t t1 = b + b;               /* 2*b  */
    seq_t t2 = t1 - a;              /* 2*b - a  */
    seq_t c  = a * t2;              /* F(2k)   = a*(2*b - a) */
    seq_t d  = a * a + b * b;       /* F(2k+1) = a*a + b*b  */

    if ((n & 1) == 0) {
        *fn  = c;                   /* even */
        *fn1 = d;
    } else {
        *fn  = d;                   /* odd */
        *fn1 = c + d;
    }
}

/* Main sequence function */
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }

    /* Compute phi(n^2).  For n up to about 10⁴ the square fits in idx_t. */
    idx_t n_sq = n * n;
    seq_t phi_val = phi(n_sq);

    /* phi_val fits in idx_t for n ≤ 10⁴; cast to idx_t. */
    idx_t k = (idx_t)phi_val;

    /* Compute Fibonacci pair (F(k), F(k+1)). */
    seq_t fk, fk1;
    fib_pair(k, &fk, &fk1);

    /* Lucas number: L(k) = 2*F(k+1) – F(k). */
    seq_t result = (seq_t)2 * fk1 - fk;
    return result;
}

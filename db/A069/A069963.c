/*
 * A069963  Define C(n) by the recursion C(0) = 6*i where i^2 = -1, C(n+1) =
 *          1/(1 + C(n)), then a(n) = 6*(-1)^n/Im(C(n)) where Im(z) denotes
 *          the imaginary part of z.
 * https://oeis.org/A069963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 36*F(n)^2 + F(n+1)^2 (F is the Fibonacci sequence)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the pair (F(n),F(n+1)) using fast doubling.
   The routine works for any seq_t that supports +, *, -, and literals. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);                /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = b * 2;                    /* 2*F(k+1) */
    c = a * (two_b - a);                   /* F(2k) = F(k) * (2*F(k+1) – F(k)) */
    d = a * a + b * b;                     /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) { /* even n */
        *fn  = c;
        *fn1 = d;
    } else {             /* odd n */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);                 /* fn  = F(n),  fn1 = F(n+1) */

    seq_t fn_sq  = fn * fn;                 /* F(n)^2 */
    seq_t fn1_sq = fn1 * fn1;               /* F(n+1)^2 */

    seq_t term2 = fn_sq * 36;               /* 36*F(n)^2 */
    return term2 + fn1_sq;                  /* final formula */
}

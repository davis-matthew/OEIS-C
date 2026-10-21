/*
 * A067984  Convolution of L(n+1) := A000204(n+1) (Lucas), n>=0, with L(n+6),
 *          n>=0.
 * https://oeis.org/A067984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (29*n+18)*F(n+1) + 18*(n+1)*F(n) (F = Fibonacci)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast‑doubling: compute (F(n),F(n+1)) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);      /* a = F(k), b = F(k+1) where k = n/2 */

    seq_t two_b = b + b;           /* 2*F(k+1) */
    seq_t c = a * (two_b - a);     /* F(2k)   = F(k)*(2*F(k+1) – F(k)) */
    seq_t d = a * a + b * b;       /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if (n & 1) {
        *fn  = d;                  /* F(2k+1) */
        *fn1 = c + d;              /* F(2k+2) = F(2k) + F(2k+1) */
    } else {
        *fn  = c;                  /* F(2k)   */
        *fn1 = d;                  /* F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    seq_t F_n, F_np1;
    fib_pair(n, &F_n, &F_np1);           /* F_n = F(n),  F_np1 = F(n+1) */

    seq_t coeff1 = ((seq_t)29 * (seq_t)n) + (seq_t)18;       /* 29*n+18 */
    seq_t coeff2 = ((seq_t)18 * (seq_t)(n + 1));             /* 18*(n+1) */

    seq_t term1 = coeff1 * F_np1;
    seq_t term2 = coeff2 * F_n;

    return term1 + term2;
}

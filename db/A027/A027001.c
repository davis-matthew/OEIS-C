/*
 * A027001  a(n) = T(2*n, n+2), T given by A026998.
 * https://oeis.org/A027001
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n+5) - (4*(n+1)^3 - 10*n^2 + 7), n >= 2 Lucas(k)
 *            = 2*F_{k+1} - F_k, where F are Fibonacci numbers
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Return F_n in *Fn and F_{n+1} in *Fn1 (fast‑doubling). */
static void fib_pair(idx_t n, seq_t *Fn, seq_t *Fn1)
{
    seq_t a = 0;   /* F_0 */
    seq_t b = 1;   /* F_1 */
    if (n == 0) { *Fn = a; *Fn1 = b; return; }

    /* number of bits in n */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp) { ++bits; tmp >>= 1; }

    for (idx_t i = bits; i > 0; --i)
    {
        seq_t two_b_minus_a = (b + b) - a;          /* 2*b - a           */
        seq_t c = a * two_b_minus_a;              /* a*(2*b - a)       */
        seq_t d = a * a + b * b;                  /* a^2 + b^2         */

        if (n & ((idx_t)1 << (i-1)))               /* next bit is 1 */
        {
            a = d;
            b = c + d;
        }
        else                                      /* next bit is 0 */
        {
            a = c;
            b = d;
        }
    }

    *Fn  = a;   /* F_n   */
    *Fn1 = b;   /* F_{n+1} */
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* offset: a(2) is first term */

    /* Lucas(2*n+5) */
    idx_t m = 2 * n + 5;
    seq_t Fm, Fm1;
    fib_pair(m, &Fm, &Fm1);                 /* Fm  = F_m , Fm1 = F_{m+1} */
    seq_t lucas = (Fm1 + Fm1) - Fm;          /* Lucas(m) = 2*F_{m+1} - F_m */

    /* Polynomial part: 4*(n+1)^3 - 10*n^2 + 7 */
    seq_t np1   = (seq_t)n + 1;                 /* n+1 */
    seq_t np1_sq = np1 * np1;                  /* (n+1)^2 */
    seq_t np1_cub = np1_sq * np1;              /* (n+1)^3 */
    seq_t poly1 = 4 * np1_cub;                 /* 4*(n+1)^3 */
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t poly2 = 10 * n_sq;                   /* 10*n^2 */
    seq_t poly = poly1 - poly2 + 7;            /* 4*(n+1)^3 - 10*n^2 + 7 */

    return lucas - poly;
}

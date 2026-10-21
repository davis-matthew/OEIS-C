/*
 * A213774  Rectangular array: (row n) = b**c, where b(h) = F(h+1), c(h) =
 *          2*n-3+2*h, F=A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   the array element T(n,k) = 2*n*Fib(k+3) + Lucas(k+3) –
 *            4*(k+n+1), where Fib and Lucas are Fibonacci and Lucas numbers.
 *            Term a(n) is read from the array by falling antidiagonals:
 *            (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),…
 * Range:     u8 n <= 28, u16 n <= 171, u32 n <= 861, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   find_sum_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/find_sum_idx.h"
/* --- local helpers -------------------------------------------------- */

/* Fast doubling Fibonacci: returns F(n) and F(n+1) in *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0; /* F(0) */
    seq_t b = 1; /* F(1) */

    /* process bits from most significant to least */
    for (int i = 63; i >= 0; --i) {
        /* c = F(2k) = a * (2*b - a) */
        seq_t t = b + b; /* 2*b  (wraps modulo seq_t) */
        t -= a;
        seq_t c = a * t;

        /* d = F(2k+1) = a*a + b*b */
        seq_t d = a * a + b * b;

        if (((n >> i) & 1ULL) == 0) {
            a = c;
            b = d;
        } else {
            a = d;
            b = c + d;
        }
    }

    *fn  = a; /* F(n)   */
    *fn1 = b; /* F(n+1) */
}

/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1, but not used */

    /* locate coordinates (row, col) by falling antidiagonal order */
    idx_t s = find_sum_idx(n);                     /* sum = row + col             */
    idx_t prev = ((s - 2) * (s - 1)) / 2;           /* terms before this diagonal */
    idx_t offset = n - prev;                        /* 1‑based position on diag  */
    idx_t row = offset;                             /* row is 1‑based             */
    idx_t col = s - row;                            /* col is 1‑based             */

    /* Fibonacci numbers needed: F_{k+2} and F_{k+3} */
    seq_t Fk2, Fk3;
    fib_pair(col + 2, &Fk2, &Fk3);                  /* F_{k+2}, F_{k+3}          */

    /* compute the term according to the closed formula */
    seq_t term = ((seq_t)(row * 2) + 1) * Fk3;       /* (2*row+1)*F_{k+3}          */
    term += Fk2 + Fk2;                              /* + 2*F_{k+2}                */
    term -= 4 * ((seq_t)(row + col + 1));           /* -4*(k+n+1)                 */

    return term;
}

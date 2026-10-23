/*
 * A165969  Triangle read by rows: T(n,m) = A094310(n,m)*A120070(n+1,m), 1 <=
 *          m <= n.
 * https://oeis.org/A165969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(k,m) where * T(k,m) = C(k,m) * (m-1)! * (k+1-m)! *
 *            (k+1+m)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Bring in the helper routines. */
#include "../utils/binom.h"      /* seq_t binom(idx_t a, idx_t b);          */
#include "../utils/factorial.h"  /* seq_t factorial(idx_t n);              */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b);        */

seq_t a(idx_t n)
{
    /* Find the row k such that the triangular index lies in that row. */
    idx_t k = 1;
    while (1) {
        idx_t prev = k * (k - 1) / 2;  /* total terms before this row */
        if (n <= prev + k)
            break;                    /* n belongs to row k */
        ++k;
    }

    /* Column position within the row. */
    idx_t m = n - k * (k - 1) / 2;    /* 1 ≤ m ≤ k */

    /* Compute the triangle entry using the closed formulas. */
    seq_t bin   = binom(k, m);                /* C(k,m)          */
    seq_t fac1  = factorial(m - 1);           /* (m-1)!          */
    seq_t fac2  = factorial(k + 1 - m);       /* (k+1-m)!        */

    seq_t res = seq_mul(bin, fac1);           /* C(k,m) * (m-1)! */
    res = seq_mul(res, fac2);                 /* * (k+1-m)!       */
    res = seq_mul(res, (seq_t)(k + 1 + m));    /* * (k+1+m)        */

    return res;
}

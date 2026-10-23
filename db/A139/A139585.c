/*
 * A139585  A triangle of coefficients from truncated Pascal binomials in
 *          {x,y},y,z}and {z,x} in which the x^n,y^n and z^n terms are
 *          deleted: f(x,y,n)=(x+y)^n-(x^n+y^n);
 *          p(x,y,z,n)=f(x,y,n)+f(y,z,n)+f(z,x,n).
 * https://oeis.org/A139585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A139585
 *
 *  The triangle is formed from the coefficients of
 *    p(x,1,1,n) = 2*(x+1)^n + 2^n – 2*x^n – 4
 *  This polynomial has degree n–1 for n≥1.
 *  Row n (starting with n=0) contains the coefficients of
 *  that polynomial in ascending powers of x, i.e.
 *    [p_n,0, p_n,1, … , p_n,n-1].
 *
 *  The sequence a(n) is the concatenation of these rows for n=0,1,2,…
 *
 *  Explicit formula for the terms (offset 1):
 *
 *    a(1) = –3                           // n = 0, constant term only
 *    a(2) =  0                           // n = 1, polynomial is identically 0
 *    For n ≥ 3:
 *        let k = n-1                     // row index (0‑based)
 *        let m = n-1 - (1 + (k-1)*k/2)   // position within the row (0‑based)
 *        if (m == 0)
 *            a(n) = 2^k – 2
 *        else
 *            a(n) = 2 * C(k, m)
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp) */
#include "../utils/binom.h"      /* seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    if (n == 1)          /* first term, row n = 0 */
        return (seq_t)(-3);

    if (n == 2)          /* second term, row n = 1 (zero polynomial) */
        return (seq_t)0;

    /* locate the row (k = n-1) and position (m) */
    idx_t row = 0;                     /* current row index (k) */
    idx_t start = 1;                   /* first index of current row */
    idx_t len   = 1;                   /* length of current row        */
    while (n > start + len - 1) {
        row++;
        start += len;
        len = (row == 0 ? 1 : (row == 1 ? 1 : row));
    }
    /* m is 0‑based position within the row */
    idx_t m = n - start;

    /* special handling for rows 0 and 1 is already done above */

    /* rows for k ≥ 2   (i.e. n ≥ 3) */
    if (m == 0) {
        /* 2^k - 2 */
        seq_t val = pow2(row);
        val -= (seq_t)2;
        return val;
    } else {
        /* 2 * C(k, m) */
        seq_t val = binom(row, m);
        val *= (seq_t)2;
        return val;
    }
}

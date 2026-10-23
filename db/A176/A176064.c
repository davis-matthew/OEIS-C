/*
 * A176064  A symmetrical triangle of polynomial coefficients based on the
 *          Hermite polynomials with leading coefficient adjusted to one:
 *          p(x,n)=HermiteH[n,x]-HermiteH[0,x]+x^n*(HermiteH[n,1/x]-HermiteH[0,1/x]).
 * https://oeis.org/A176064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 35, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176064 – triangular array values via Hermite polynomial coefficients.
   Computed as:
       out(n,m) = h_{n,m} + h_{n,n-m} - h_{n,0} - h_{n,n}
                  - (m==0?1:0) - (m==n?1:0) + 2
   where h_{n,m} is the coefficient of x^m in the Hermite polynomial H_n(x)
   (physicists’ convention).  See description above. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t) */

/* Return the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2  */
static idx_t row_of(idx_t n)
{
    idx_t low = 0, high = n;          /* high can be at most n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* use 128-bit arithmetic to avoid overflow */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Coefficient of x^m in the physicists Hermite polynomial H_n(x). */
static seq_t hermite_coef(idx_t n, idx_t m)
{
    /* non‑zero only when n and m have the same parity */
    if (((n - m) & 1) != 0)
        return (seq_t)0;

    idx_t k = (n - m) >> 1; /* (n-m)/2 */
    seq_t res = (seq_t)1;

    /* product_{i=k+1}^{n} i */
    for (idx_t i = k + 1; i <= n; ++i)
        res *= (seq_t)i;

    /* divide by m! */
    for (idx_t i = 1; i <= m; ++i)
        res /= (seq_t)i;

    /* factor (-1)^((n-m)/2) */
    if (k & 1)
        res = -res;

    /* multiply by 2^m */
    if (m > 0)
        res *= seq_pow((seq_t)2, m);

    return res;
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* locate the row and column of the triangle */
    idx_t r = row_of(n);
    idx_t c = n - r * (r + 1) / 2;   /* 0 <= c <= r */

    /* coefficients needed */
    seq_t h0   = hermite_coef(r, (idx_t)0);          /* constant term */
    seq_t hr   = seq_pow((seq_t)2, r);               /* leading term */
    seq_t hc   = hermite_coef(r, c);
    seq_t hrc  = hermite_coef(r, r - c);

    /* compute the adjusted out value as described */
    seq_t out = hc + hrc - h0 - hr;

    if (c == 0)
        out -= (seq_t)1;
    if (c == r)
        out -= (seq_t)1;
    out += (seq_t)2;

    return out;
}

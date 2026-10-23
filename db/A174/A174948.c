/*
 * A174948  A symmetrical triangle sequence based
 *          on:q=1/12;t(n,m,q)=12*(Binomial[n, m]*(1 - q) + (((n + m + 1)!/((n
 *          + 1)!* m!)) + ((2*n - m + 1)!/((n + 1)!*(n - m)!)))*q).
 * https://oeis.org/A174948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   a(n) = out(row, col) where * row = floor((sqrt(8n+1)-1)/2) * col
 *            = n - row*(row+1)/2 * out(n,m) = t(n,m) - t(n,0) + 1 * with
 *            t(n,m) = 11*Binomial(n,m) + Binomial(n+m+1,m) + Binomial(2n-m+1,
 *            n-m) * and Binomial(x,y) = x choose y (integer)
 * Range:     i8 n <= 15, i16 n <= 45, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static inline seq_t binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t r = 1;
    for (idx_t i = 1; i <= b; ++i) {
        r = r * (a - i + 1) / i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find the row number r such that n is in row r (0‑based). */
    idx_t r = (idx_t)((sqrt((long double)(8.0L * n + 1)) - 1.0L) / 2.0L);
    /* Correct possible rounding errors. */
    if (r * (r + 1) / 2 > n) {
        while (r * (r + 1) / 2 > n) r--;
    } else {
        while ((r + 1) * (r + 2) / 2 <= n) r++;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t m = n - start;            /* column within the row */

    /* Compute the triangle entry. */
    seq_t t = 11 * binom(r, m);
    t += binom(r + m + 1, m);
    t += binom(2 * r - m + 1, r - m);

    /* t(n,0) uses the same formula with m = 0. */
    seq_t t0 = 12 + binom(2 * r + 1, r);

    return t - t0 + 1;
}

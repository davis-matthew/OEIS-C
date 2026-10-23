/*
 * A180047  Coefficient triangle of the numerators of the (n-th convergents
 *          to) the continued fraction w/(1 + w/(2 + w/(3 + w/(...)))).
 * https://oeis.org/A180047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,c) where the triangle T(r,c) is * * T(r,c) = (r‑c+1)!
 *            / c! * binomial(r‑c, c‑1) * * r ≥ 0, 0 ≤ c ≤ (r+1)/2. * * a(n)
 *            is the row‑wise flattening of the triangle starting with *
 *            T(0,0) = 0. Thus a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=2, …
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd, range_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/range_prod.h"

/* Binomial coefficient C(n,k) computed with cancellation to avoid overflow. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numer = (seq_t)(n - k + i);
        seq_t den   = (seq_t)i;

        /* Cancel any common factors between numer and den. */
        seq_t g = gcd(numer, den);
        numer /= g;
        den   /= g;

        /* Cancel any common factors between res and den. */
        g = gcd(res, den);
        res /= g;
        den  /= g;

        /* Multiply res by the remaining numerator and divide by the remaining denominator. */
        res = res * numer;
        res = res / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Locate row r and column c such that the flattened index n corresponds to T(r,c). */
    idx_t row = 0;
    idx_t remaining = n;
    idx_t len = (row + 1) / 2 + 1;          /* number of elements in current row */
    while (remaining >= len)
    {
        remaining -= len;
        ++row;
        len = (row + 1) / 2 + 1;
    }
    idx_t col = remaining;                   /* zero‑based column index */

    /* The first element of each row is always 0. */
    if (col == 0)
        return (seq_t)0;

    /* Compute T(row,col). */
    seq_t bin = binom_calc(row - col, col - 1);
    seq_t fact_part = range_prod(col + 1, row - col + 1);

    return fact_part * bin;
}

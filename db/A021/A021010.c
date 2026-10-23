/*
 * A021010  Triangle of coefficients of Laguerre polynomials L_n(x) (powers of
 *          x in decreasing order).
 * https://oeis.org/A021010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, nice
 * Formula:   a(n) = (-1)^(r-c) * c! * (C(r,c))^2 where (r,c) are obtained by
 *            triangular indexing: r = floor((-1 + sqrt(8n+1)) / 2), c = n -
 *            r(r+1)/2
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 84, i64 n <= 203,
 *            i128 n <= 552, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for idx_t (non‑negative) */
static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* row number */
    idx_t discr = 8 * n + 1;
    idx_t s     = isqrt_idx(discr);
    idx_t r     = (s - 1) >> 1;                 /* (s-1)/2 */

    /* column number */
    idx_t prev = (idx_t)(((uint64_t)r * (r + 1)) >> 1);   /* r(r+1)/2 */
    idx_t c    = n - prev;

    /* binomial(r,c) */
    idx_t k = c <= r - c ? c : r - c;
    seq_t bin = 1;
    for (idx_t j = 1; j <= k; ++j)
    {
        seq_t num = (seq_t)(r - k + j);
        bin = bin * num / j;    /* exact division */
    }

    /* factorial of c */
    seq_t fact = 1;
    for (idx_t j = 1; j <= c; ++j)
        fact = fact * (seq_t)j;

    /* combine */
    seq_t result = fact * bin * bin;
    if ((r - c) & 1)
        result = -result;
    return result;
}

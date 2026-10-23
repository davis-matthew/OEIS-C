/*
 * A118401  Triangle, read by rows, equal to the matrix square of triangle
 *          A118400; also equals the matrix inverse of triangle A118407.
 * https://oeis.org/A118401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coeff of x^(r‑c) in (1+2x+3x^2+2x^3+2x^4)*(1+x)^{-2},
 *            where r and c are obtained from n by interpreting n as the (r,c)
 *            entry of the triangle read by rows
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Numerator coefficients of (1+2x+3x^2+2x^3+2x^4) */
static const seq_t NUM[5] = {(seq_t)1,(seq_t)2,(seq_t)3,(seq_t)2,(seq_t)2};

/* Compute f(k) = \sum_{i=0}^{min(4,k)} a_i * (-1)^(k-i) * (k-i+1) */
static seq_t f(idx_t k)
{
    seq_t res = 0;
    idx_t limit = (k < 4 ? k : 4);
    for (idx_t i = 0; i <= limit; ++i)
    {
        seq_t coeff = NUM[i];
        idx_t d = k - i;                          /* k-i >= 0 */
        seq_t term = (seq_t)((__int128)coeff * (d + 1));
        if (d & 1) term = -term;
        res += term;
    }
    return res;
}

/* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Exponential search for an upper bound */
    while ( ((__int128)hi * (hi + 1)) / 2 <= n )
        hi <<= 1;
    /* Binary search between lo and hi */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ( ((__int128)mid * (mid + 1)) / 2 <= n )
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t start = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t c = n - start;     /* column index, 0 <= c <= r */
    idx_t k = r - c;         /* argument for f(k) */
    return f(k);
}

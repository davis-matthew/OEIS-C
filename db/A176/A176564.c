/*
 * A176564  Triangle T(n,m)= binomial(2*n,m) + binomial(2*n,n-m)
 *          -binomial(2*n,n) read by rows.
 * https://oeis.org/A176564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 36, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r,m) where r = floor((sqrt(8*n+1)-1)/2)
 * and m = n - r*(r+1)/2.
 * T(r,m) = binom(2*r,m) + binom(2*r,r-m) - binom(2*r,r).
 *
 * Computes the OEIS sequence A176564 (triangle read by rows).
 */
#include <math.h>           /* sqrt needed for the upper bound of r */
#include "../utils/binom.h" /* provides seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t lo = 0;
    idx_t hi;

    /* Rough upper bound: r ≈ sqrt(2*n). */
    if (n == 0) {
        hi = 0;
    } else {
        double d = sqrt((double)2.0 * (double)n) + 3.0;
        hi = (idx_t)d;
    }

    /* Binary search for largest r with r*(r+1)/2 <= n */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    idx_t r = hi;                     /* correct r */
    idx_t prev = r * (r + 1) / 2;     /* elements before row r */
    idx_t m = n - prev;               /* column index in row r */

    idx_t two_r = r << 1;             /* 2*r */

    seq_t term1 = binom(two_r, m);
    seq_t term2 = binom(two_r, r - m);
    seq_t term3 = binom(two_r, r);

    return term1 + term2 - term3;
}

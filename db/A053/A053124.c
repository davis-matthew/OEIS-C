/*
 * A053124  Triangle of coefficients of Chebyshev's U(n,2*x-1) polynomials
 *          (exponents of x in increasing order).
 * https://oeis.org/A053124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, sign, tabl
 * Formula:   * a(k) = (4^m) * (-1)^(r-m) * C(r+m+1, 2m+1), * where r =
 *            floor((sqrt(8k+1)-1)/2) (row index), * m = k - r(r+1)/2 (column
 *            index)
 * Range:     i8 n <= 11, i16 n <= 31, i32 n <= 99, i64 n <= 367,
 *            i128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binom is provided by the build utilities. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t lo = 0;
    idx_t hi = n;                    /* hi can never be smaller than r  */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;

    /* Column index m */
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t m = (idx_t)(n - (idx_t)tri_r);

    /* Compute the combinatorial factor */
    idx_t a_arg = r + m + 1;        /* n + m + 1 in formula */
    idx_t b_arg = 2 * m + 1;        /* 2m + 1 in formula */
    seq_t binom_val = binom(a_arg, b_arg);

    /* Compute 4^m */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < m; ++i)
        pow4 *= 4;

    /* Combine everything */
    seq_t result = binom_val * pow4;
    if (((r - m) & 1) != 0)
        result = -result;

    return result;
}

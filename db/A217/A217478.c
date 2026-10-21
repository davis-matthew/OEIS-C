/*
 * A217478  Triangle of coefficients of polynomials providing the second term
 *          of the numerator for the generating function for odd powers
 *          (2*m+1) of Chebyshev S-polynomials. The present polynomials are
 *          called P(m;1,x^2).
 * https://oeis.org/A217478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = (-1)^(m-k)*C(m+k+1, 2k+1), where m is the row number (m ≥
 *            1) satisfying (m-1)m/2 < n ≤ m(m+1)/2 and k = n – (m-1)m/2 – 1
 * Range:     i8 n <= 12, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* Find the row m such that triangular number T(m-1) < n ≤ T(m) */
    idx_t low = 1, high = 2;

    /* Increase high until T(high) ≥ n, using 128‑bit to avoid overflow */
    while ( ((unsigned __int128)high * (high + 1)) / 2 < n )
        high *= 2;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ((unsigned __int128)mid * (mid + 1)) / 2 >= n )
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t m = low;

    /* Compute k within the row */
    idx_t prev = (m - 1) * m / 2;           /* T(m-1) */
    idx_t k   = n - prev - 1;               /* 0 ≤ k < m */

    /* Binomial coefficient binomial(m + k + 1, 2k + 1) */
    idx_t bn = m + k + 1;
    idx_t bk = 2 * k + 1;
    seq_t bin = binomial(bn, bk);

    /* Sign factor (-1)^(m - k) */
    seq_t sign = ((m - k) & 1) ? -1 : 1;

    return bin * sign;
}

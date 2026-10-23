/*
 * A049310  Triangle of coefficients of Chebyshev's S(n,x) := U(n,x/2)
 *          polynomials (exponents in increasing order).
 * https://oeis.org/A049310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, sign, tabl, core, changed
 * Formula:   a(n) is the n‑th entry of the triangle of coefficients of the
 *            Chebyshev polynomials S(n,x)=U(n,x/2) arranged row by row. For
 *            an index n the row r and column k are r =
 *            floor((sqrt(8*n+1)-1)/2) k = n - r*(r+1)/2 The triangle entry is
 *            T(r,k) = 0 if r < k or (r+k) odd T(r,k) = (-1)^( (r+k)/2 + k )
 *            *...
 * Range:     i8 n <= 70, i16 n <= 360, i32 n <= 1245, i64 n <= 4695,
 *            i128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

static idx_t row_of(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2) with a tiny adjustment. */
    long double root = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((root - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one errors caused by floating‑point rounding. */
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* Triangle entry */
    if (r < k || ((r + k) & 1U)) /* n+k odd */
        return 0;

    idx_t m = (r + k) / 2;                   /* (r+k)/2 */
    seq_t bin = binomial(m, k);              /* binomial((r+k)/2 , k) */
    seq_t sign = ((m + k) & 1U) ? (seq_t)-1 : (seq_t)1;

    return sign * bin;
}

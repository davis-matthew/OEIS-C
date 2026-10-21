/*
 * A084930  Triangle of coefficients of Chebyshev polynomials T_{2n+1} (x).
 * https://oeis.org/A084930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = (-1)^(r-k) * 2^(2*k) * binom(r+1+k, 2*k+1) *
 *            (2*r+1)/(r+1+k) where n = r*(r+1)/2 + k, 0 <= k <= r
 * Range:     i8 n <= 10, i16 n <= 31, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that (r+1)*(r+2)/2 > n.
       The triangle has (r+1) terms in row r, so the total number
       of terms up to and including row r is (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    /* The number of terms before this row. */
    idx_t prev = r * (r + 1) / 2;

    /* Column index within the row. */
    idx_t k = n - prev;

    /* Compute the coefficient using the closed‑form formula. */
    seq_t val = binom(r + 1 + k, 2 * k + 1);
    val *= (seq_t)(2 * r + 1);
    val /= (seq_t)(r + 1 + k);

    /* Multiply by 4^k (2^(2*k)). */
    for (idx_t i = 0; i < k; ++i) {
        val *= 4;
    }

    /* Apply the sign (-1)^(r-k). */
    if ((r - k) & 1) {
        val = -val;
    }

    return val;
}

/*
 * A128256  Matrix product of (-1)^(n-k) * A004736(n+1,k+1) with A007318
 *          (Pascal's triangle).
 * https://oeis.org/A128256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = Σ_{j=0}^{r-k} (-1)^j (j+1) * binom(r-j,k) * where r is
 *            the unique integer such that r(r+1)/2 ≤ n < * (r+1)(r+2)/2 and k
 *            = n - r(r+1)/2. * This is the row‑wise enumeration of the
 *            infinite lower‑triangular * array T(r,k) with T(r,k) =
 *            Σ_{j=0}^{r-k} (-1)^j (j+1) * C(r-j,k)
 * Range:     i8 n <= 38, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k within that row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r,k) = Σ_{j=0}^{r-k} (-1)^j (j+1) * binom(r-j,k). */
    seq_t sum = 0;
    for (idx_t j = 0; j <= r - k; ++j)
    {
        seq_t term = binom(r - j, k);          /* binomial coefficient */
        term *= (seq_t)(j + 1);                /* multiply by (j+1) */
        if (j & 1)                             /* (-1)^j: odd j -> subtract */
            sum -= term;
        else
            sum += term;
    }
    return sum;
}

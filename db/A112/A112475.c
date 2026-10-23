/*
 * A112475  Riordan array (1/(1+x),x(1+x)/(1-x)).
 * https://oeis.org/A112475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the row‑major entry T(r,k) of the Riordan array
 *            (1/(1+x), x(1+x)/(1‑x)), where r = floor((sqrt(8n+1)-1)/2) and k
 *            = n – r(r+1)/2. T(n,k) = Σ_{j=0}^{n-k} C(k-1,j) * C(n-j-1,n-k-j)
 * Range:     i8 n <= 39, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2) / 2) <= n)
        ++r;

    /* Column index within this row. */
    idx_t start = ((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;

    /* For k = 0 we have the closed form T(r,0) = (-1)^r. */
    if (k == 0)
        return (seq_t)((r & 1) ? -1 : 1);

    /* Otherwise compute the sum of binomial products. */
    seq_t sum = 0;
    for (idx_t j = 0; j <= r - k; ++j)
    {
        seq_t c1 = binom(k - 1, j);
        seq_t c2 = binom(r - j - 1, r - k - j);
        sum += c1 * c2;
    }
    return sum;
}

/*
 * A116405  Triangle whose k-th column has e.g.f. sum{j=0..k,
 *          (-1)^j*Bessel_I(k+j,2x)}.
 * https://oeis.org/A116405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the row‑wise flattening of the triangle T(r,c) where
 *            T(r,c) = Σ_{j=0..c} (-1)^j · binomial(r, (r-(c+j))/2) (with
 *            binomial interpreted as 0 when the lower index is not an integer
 *            or out of bounds). The indices r, c are defined by n = r(r+1)/2
 *            + c, 0 ≤ c ≤ r
 * Range:     i8 n <= 35, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    for (;;) {
        /* use unsigned __int128 to avoid overflow on small r */
        unsigned __int128 tmp = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (tmp > n) break;
        ++r;
    }

    /* total number of elements before row r */
    unsigned __int128 prev_total = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)prev_total;  /* column index in row r */

    seq_t sum = 0;

    for (idx_t j = 0; j <= c; ++j) {
        /* once k > r it contributes 0, break early */
        if (r < c + j) break;

        idx_t d = r - (c + j);
        if (d & 1) continue;          /* parity mismatch -> term is 0 */

        idx_t k = d >> 1;             /* (r-(c+j))/2 */
        seq_t term = binomial(r, k);   /* binomial(r, k) is zero if k>r */

        if (j & 1U) term = -term;      /* multiply by (-1)^j */

        sum += term;
    }

    return sum;
}

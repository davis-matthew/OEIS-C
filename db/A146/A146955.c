/*
 * A146955  A functionally symmetric Polynomial as a triangle of coefficients:
 *          p(x,n)=If[n == 0, 1, (x + 1)^n + 2^(n - 4)*Sum[(2^m + 2*m )*x^m*(1
 *          + x^(n - 2*m)), {m, 1, n - 1}]].
 * https://oeis.org/A146955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient t(r,c) in the triangle of coefficients of the
 *            polynomial p(x,r) = (x + 1)^r + 2^(r-4) * Σ_{m=1}^{r-1} (2^m +
 *            2m) * x^m * (1 + x^(r-2m)) where r and c satisfy r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 (row index) c = n - r(r+1)/2 (column index) For 0 ≤
 *            c ≤ r : t(r,0) = t(r,r) = 1 t(r,c) = C(r...
 * Range:     u8 n <= 28, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle the trivial first term */
    if (n == 0)
        return (seq_t)1;

    /* Determine the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2
       Using a simple linear search is fast enough for the required
       range (n up to about 10 000).  The loop runs at most √(2n) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;

    /* column index within the row */
    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;

    /* boundary coefficients are 1 */
    if (c == 0 || c == r)
        return (seq_t)1;

    /* binomial coefficient C(r,c) */
    seq_t bin = binomial(r, c);

    /* part of the sum: (2^c + 2c) + (2^(r-c) + 2(r-c)) */
    seq_t pwr_c       = pow2(c);
    seq_t pwr_rc      = pow2(r - c);
    seq_t part_sum = pwr_c + (seq_t)2 * c + pwr_rc + (seq_t)2 * (r - c);

    /* multiply by 2^(r-4); for r<4 use division (exact) */
    seq_t add;
    if (r >= 4) {
        add = part_sum * pow2(r - 4);
    } else {
        add = part_sum / pow2(4 - r);
    }

    return bin + add;
}

/*
 * A217476  Coefficient triangle for the square of the monic integer Chebyshev
 *          T-polynomials A127672.
 * https://oeis.org/A217476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r,k) where r(r+1)/2 ≤ n < (r+1)(r+2)/2, k = n –
 *            r(r+1)/2. T(0,0) = 4. For r≥1: T(r,k) = 2*[k=0] +
 *            2*r*(-1)^(r-k)*binomial(r+k,r-k)/(r+k) with binomial(m,n) from
 *            the provided utilities
 * Range:     i8 n <= 7, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the only row 0 entry */
    if (n == 0) return (seq_t)4;

    /* Determine which row r the global index n lies in */
    seq_t idx = (seq_t)n;
    seq_t d   = (seq_t)1 + (seq_t)8 * idx;
    seq_t sq  = floor_sqrt(d);
    seq_t r   = (sq - (seq_t)1) / (seq_t)2;

    /* Offset to the start of row r */
    seq_t offset = r * (r + (seq_t)1) / (seq_t)2;

    /* Column within the row */
    seq_t k = idx - offset;
    seq_t rrow = r;

    /* First part: 2 if k==0 else 0 */
    seq_t part1 = (k == (seq_t)0) ? (seq_t)2 : (seq_t)0;

    /* Sign factor (-1)^(r-k) */
    seq_t sign = ((rrow - k) & (seq_t)1) == (seq_t)0 ? (seq_t)1 : (seq_t)-1;

    /* Binomial coefficient binomial(r+k, r-k) */
    seq_t bin = binomial((idx_t)(rrow + k), (idx_t)(rrow - k));

    /* Multiply by 2*r*sign and divide by (r+k) */
    seq_t mult   = (seq_t)2 * rrow * sign;
    seq_t part2  = mult * bin / (rrow + k);

    return part1 + part2;
}

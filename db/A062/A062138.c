/*
 * A062138  Coefficient triangle of generalized Laguerre polynomials
 *          n!*L(n,5,x)(rising powers of x).
 * https://oeis.org/A062138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r,c) where r and c satisfy n = r*(r+1)/2 + c, and
 *            T(r,c) = ((-1)^c) * r! * binomial(r+5, r-c) / c! (OEIS A062138)
 * Range:     i8 n <= 5, i16 n <= 20, i32 n <= 54, i64 n <= 152, i128 n <= 465,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column c from index n */
    idx_t r = 0;
    /* cumulative number of elements up to row r is r*(r+1)/2 */
    /* Find maximum r such that r*(r+1)/2 <= n */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;              /* column index in this row */

    /* Compute binomial part: C(r+5, r-c) */
    seq_t bin = binomial(r + 5, r - c);

    /* Compute r! / c! = product_{i=c+1}^{r} i */
    seq_t fact = 1;
    for (idx_t i = c + 1; i <= r; ++i)
        fact *= (seq_t)i;

    /* Combine with sign */
    seq_t result = fact * bin;
    if (c & 1)  /* odd c => negate */
        result = -result;

    return result;
}

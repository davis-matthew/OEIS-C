/*
 * A230207  Trapezoid of dot products of row 4 (signs alternating) with
 *          sequential 5-tuples read by rows in Pascal's triangle A007318:
 *          T(n,k) is the linear combination of the 5-tuples (C(4,0), -C(4,1),
 *          C(4,2), -C(4,3), C(4,4)) and (C(n-1,k-4), C(n-1,k-3), C(n-1,k-2),
 *          C(n-1,k-1), C(n-1,k)), n >= 1, 0 <= k <=...
 * https://oeis.org/A230207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = flatten row-wise of the trapezoid T(n,k) * where T(n,k) =
 *            Σ_{j=0..4} (-1)^j * binom(4,j) * binom(n-1, k-4+j), * with
 *            binom(a,b)=0 if b<0 or b>a. This implements the definition * of
 *            OEIS sequence A230207
 * Range:     i8 n <= 71, i16 n <= 352, i32 n <= 796, i64 n <= 1475,
 *            i128 n <= 1475, big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the first r rows contain at least n terms.
       The cumulative length of the first r rows is (r^2 + 9r)/2.
       Since r grows only like O(√n), a linear search is fast enough. */
    idx_t r = 1;
    while ((r * r + 9 * r) / 2 < n) ++r;

    /* Number of terms before row r. */
    idx_t prev = ((r - 1) * r) / 2 + 4 * (r - 1);
    /* Position within row r (0‑based). */
    idx_t k = n - prev - 1;

    seq_t result = 0;
    idx_t row_minus1 = r - 1;

    for (idx_t j = 0; j <= 4; ++j) {
        /* i = k - 4 + j; skip if i would be negative or larger than row_minus1. */
        if (k < (4 - j)) continue;          /* i would be negative */
        idx_t i = k - 4 + j;                /* guaranteed non‑negative here */
        if (i > row_minus1) continue;       /* outside the valid binomial range */

        seq_t comb  = binomial(row_minus1, i);
        seq_t coeff = binomial(4, j);
        seq_t term  = seq_mul(coeff, comb);

        if (j & 1) result -= term;   /* odd j → negative sign */
        else      result += term;    /* even j → positive sign */
    }

    return result;
}

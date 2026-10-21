/*
 * A086617  Symmetric square table of coefficients, read by antidiagonals,
 *          where T(n,k) is the coefficient of x^n*y^k in f(x,y) that
 *          satisfies f(x,y) = 1/((1-x)(1-y)) + xy*f(x,y)^2.
 * https://oeis.org/A086617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{t=0}^{min(i,j)} C(i,t) · C(j,t) · Catalan(t) where
 *            (i,j) are the coordinates of the antidiagonal that contains
 *            index n. Let d satisfy d·(d+1)/2 ≤ n < (d+1)·(d+2)/2. Then
 *            offset = n – d·(d+1)/2, i = d – offset, j = offset
 * Range:     u8 n <= 48, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the necessary helper functions. */
#include "../utils/binom.h"
#include "../utils/catalan.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d that contains index n. */
    idx_t d = 0;
    while ( ((__int128)(d + 1) * (d + 2)) / 2 <= n )
        ++d;

    /* Compute the starting index of diagonal d. */
    idx_t base = (idx_t)(((__int128)d * (d + 1)) / 2);

    /* Offset of n inside diagonal d. */
    idx_t offset = n - base;

    /* Coordinates (i,j) on the antidiagonal. */
    idx_t i = d - offset;
    idx_t j = offset;

    /* Sum over t = 0 .. min(i,j).  */
    idx_t limit = (i < j) ? i : j;
    seq_t sum = 0;
    for (idx_t t = 0; t <= limit; ++t) {
        seq_t v = binom(i, t);
        v *= binom(j, t);
        v *= catalan(t);
        sum += v;
    }
    return sum;
}

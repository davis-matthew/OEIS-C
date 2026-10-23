/*
 * A027466  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j).
 * https://oeis.org/A027466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (n)-th entry of the triangle T(i,j) = binomial(i,j)
 *            * 7^(i-j); Locate row i and column j from the linear index n
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 77, u64 n <= 275, u128 n <= 992,
 *            big any n
 * Helpers:   binomial, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Locate the row (i) containing the n‑th entry using the
       inverse of the triangular number sum.  The formula
       i = floor((sqrt(8*n+1)-1)/2).  We use double for the
       initial approximation and then fix any rounding error
       with a couple of corrections. */
    double dn = (double)n;
    double approx = sqrt( 8.0 * dn + 1.0 );
    idx_t i = (idx_t)((approx - 1.0) / 2.0);

    /* Correct any possible off-by-one errors caused by double
       rounding.  This loop runs at most a few times. */
    while ( ( (__int128)(i + 1) * (i + 2) / 2 ) <= n ) i++;
    while ( ( (__int128)i * (i + 1) / 2 ) > n ) i--;

    /* Column index within the found row. */
    idx_t j = n - ( (__int128)i * (i + 1) / 2 );

    /* Compute binomial coefficient and the power of 7. */
    seq_t bin = binomial(i, j);
    seq_t pow7 = pow_uint(7, i - j);

    /* Return the product.  seq_mul works for both builtin integers
       and arbitrary‑precision types. */
    return seq_mul(bin, pow7);
}

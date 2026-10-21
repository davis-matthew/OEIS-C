/*
 * A053120  Triangle of coefficients of Chebyshev's T(n,x) polynomials (powers
 *          of x in increasing order).
 * https://oeis.org/A053120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice, easy, changed
 * Formula:   a(n) = T(r,c) where r and c are obtained from the linear index n
 *            by row = largest r with r*(r+1)/2 <= n c = n - r*(r+1)/2 T(r,c)
 *            are the Chebyshev T(r,x) coefficients in increasing powers of x.
 *            T(r,c) is defined by: T(r,c) = 0 if r < c or r+c is odd T(r,0) =
 *            (-1)^(r/2) if r even T(r,c) = (-1)^( (...
 * Range:     i8 n <= 39, i16 n <= 114, i32 n <= 396, i64 n <= 1463,
 *            i128 n <= 5150, big any n
 * Helpers:   chebyshev_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/chebyshev_T.h"

seq_t a(idx_t n)
{
    /* determine row r and column c from the linear index n */
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n ) r++;
    idx_t c = n - r * (r + 1) / 2;

    return chebyshev_T(r, c);
}

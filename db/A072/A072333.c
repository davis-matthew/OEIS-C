/*
 * A072333  Largest eigenvalue, rounded to the nearest integer, of a rank n
 *          matrix of 1..n^2 filled successively along rows.
 * https://oeis.org/A072333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( ( n^3 + n ) / 4 + sqrt( ( n^3 + n )^2 / 4 + n^3
 *            (n^2 - 1) / 3 ) / 2 ) which follows from the eigenvalues of the
 *            rank‑2 matrix A(i,j)=n*(i‑1)+j (trace = (n³+n)/2, determinant =
 *            –n³(n²−1)/12). The returned value is the largest eigenvalue
 *            rounded to the nearest integer
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   int128_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/int128_sqrt.h"

seq_t a(idx_t n)
{
    /* Work in 128 bits to avoid overflow for n up to about 10000. */
    unsigned __int128 nn = n;

    /* trace = (n³ + n) / 2 */
    unsigned __int128 tr = (nn * nn * nn + nn) / 2U;

    /* determinant = – n³ (n² – 1) / 12 (negative number) */
    signed __int128 det = - (signed __int128)(nn * nn * nn) * (signed __int128)(nn * nn - 1U) / 12;

    /* Compute D = tr² – 4*det = tr² + 4*|det| (det is negative) */
    unsigned __int128 neg_det = (unsigned __int128)(-det);          /* |det| as unsigned */
    unsigned __int128 D = tr * tr + (unsigned __int128)4U * neg_det;

    /* Integer square root of D */
    unsigned __int128 s = int128_sqrt(D);

    /* Largest eigenvalue rounded to nearest integer:
       a(n) = floor( (tr + s + 1) / 2 )  */
    unsigned __int128 res = (tr + s + 1U) / 2U;

    return (seq_t)res;
}

/*
 * A172417  Triangle read by rows: Catalan number C(n) repeated n times.
 * https://oeis.org/A172417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = Catalan( ceil( (sqrt(8*n+1)-1)/2 ) )
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   catalan, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; undefined for n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* X = 8*n + 1  (use seq_t to avoid overflow on large n) */
    seq_t X = (seq_t)8 * (seq_t)n + (seq_t)1;

    /* sqrtX = floor( sqrt( X ) );  seq_t to support big values */
    seq_t sqrtX = floor_sqrt(X);

    /* r = ceil( (sqrtX - 1) / 2 )  implemented by first getting floor and
       then correcting if the triangular number is too small          */
    idx_t r = ((idx_t)sqrtX - 1) / 2;

    /* T(r) = r * (r + 1) / 2 ; if T(r) < n, then r needs to be increased */
    seq_t T = (seq_t)r * (r + 1) / 2;
    if (T < (seq_t)n)
        r++;

    /* Return the Catalan number for this row */
    return catalan(r);
}

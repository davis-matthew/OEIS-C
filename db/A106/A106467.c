/*
 * A106467  Inverse of number triangle A106465.
 * https://oeis.org/A106467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the element of the lower‑triangular number triangle
 *            where each even column 2k has the pattern 1, –1, –1, 1 starting
 *            at row r = 2k, and each odd column 2k+1 has the pattern 1, –1
 *            starting at row r = 2k+1. The triangle is read row‑by‑row to
 *            produce the sequence
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ( (( (__int128)(r) * (r + 1)) >> 1 ) <= n )
        r++;
    r--;                                    /* r now satisfies the inequality */

    /* Offset of the start of row r */
    idx_t offset = ( (__int128)r * (r + 1) ) >> 1;
    idx_t c = n - offset;                  /* column index within the row */

    /* Compute value according to column parity and position */
    seq_t val = 0;
    if ( (c & 1) == 0 ) {                  /* even column 2k */
        if ( r == c )          val =  1;
        else if ( r == c + 1 )  val = -1;
        else if ( r == c + 2 )  val = -1;
        else if ( r == c + 3 )  val =  1;
        else                   val =  0;
    } else {                               /* odd column 2k+1 */
        if ( r == c )          val =  1;
        else if ( r == c + 2 ) val = -1;
        else                   val =  0;
    }

    return val;
}

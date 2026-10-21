/*
 * A204447  Symmetric matrix: f(i,j)=floor[(i+j+5)/4]-floor[(i+j+2)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   _first_t_ge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204447: values of the symmetric 0/1 matrix
 * with entries f(i,j) = floor[(i+j+5)/4] - floor[(i+j+2)/4]
 * read along successive antidiagonals.
 * a(n) is the value of the n-th entry when the matrix is
 * enumerated by antidiagonals.  Let t(n) be the smallest integer
 * t ≥ 2 with (t-1)*t/2 ≥ n.  Then
 *   a(n) = 0  if  t(n) mod 4 == 2
 *   a(n) = 1  otherwise
 */

#include <stdint.h>
#include "../utils/_first_t_ge.h"

seq_t a(idx_t n)
{
    /* n is assumed to be ≥ 1 because the sequence offset is 1 */
    idx_t t = _first_t_ge(n);

    /* Antidiagonal sum s equals t.
       The matrix entry is 0 exactly when s mod 4 == 2. */
    return ((t & 3U) == 2) ? (seq_t)0 : (seq_t)1;
}

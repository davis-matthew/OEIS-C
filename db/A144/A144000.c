/*
 * A144000  Rectangular array by antidiagonals: label each unit square in the
 *          first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares for which x + y == 0 (mod 3); then R(m,n) is the number of
 *          marked squares in the rectangle [0,m]x[0,n].
 * https://oeis.org/A144000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 343, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   antidiagonal_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144000: a(n) = R(m,j) where (m,j) is the pair of positive
   integers reached when enumerating the array by antidiagonals
   starting with (1,1).  R(m,j) = floor(2*m*j/3) except when j mod 3 == 1,
   then R(m,j) = floor((2*m*j+1)/3). */
/* The value is computed in O(sqrt(n)) time. */

#include <stdint.h>
#include "../utils/antidiagonal_pair.h"

seq_t a(idx_t n)
{
    idx_t m, j;
    antidiagonal_pair(n, &m, &j);

    seq_t prod = (seq_t)m * (seq_t)j;
    seq_t twoProd = prod + prod;              /* 2*m*j */

    if (j % 3 == 1)
        return (twoProd + 1) / 3;             /* floor((2*m*j+1)/3) */
    else
        return twoProd / 3;                   /* floor(2*m*j/3) */
}

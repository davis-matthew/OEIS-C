/*
 * A171973  Integer part of the volume of a regular tetrahedron with edge
 *          length n.
 * https://oeis.org/A171973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^3 * sqrt(2) / 12)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Computes the integer part of the volume of a regular tetrahedron
   whose edge length is n.  The calculation is performed using
   only integer arithmetic:  floor(n^3 * sqrt(2) / 12) =
   floor( sqrt(2 * (n^3)^2) ) / 12,  since n^3 * sqrt(2) =
   sqrt(2 * (n^3)^2).  The helper function floor_sqrt returns
   floor(sqrt(x)) for any seq_t value.  */
seq_t a(idx_t n)
{
    /* Promote n to seq_t once */
    seq_t N = (seq_t)n;

    /* n^2 and n^3 */
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;

    /* 2 * N3^2 = 2 * (n^3)^2 */
    seq_t sq = N3 * N3;
    sq = sq + sq;      /* multiply by 2 */

    /* floor(sqrt(2 * (n^3)^2)) */
    seq_t sqrt_val = floor_sqrt(sq);

    /* integer division by 12 gives the floor of the original expression */
    return sqrt_val / 12;
}

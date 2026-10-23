/*
 * A187338  a(n) = 3*n + floor(sqrt(2)*n), complement of A187328.
 * https://oeis.org/A187338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + floor(sqrt(2)*n) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* cast index to seq_t for all arithmetic */
    seq_t nn = (seq_t)n;

    /* first part: 3*n */
    seq_t term1 = nn * 3;

    /* second part: floor(sqrt(2)*n) = floor(sqrt(2*n*n)) */
    seq_t sq = nn * nn;        /* n^2   */
    sq = sq * 2;               /* 2*n^2 */
    seq_t fl = floor_sqrt(sq); /* floor(sqrt(2*n*n)) */

    return term1 + fl;
}

/*
 * A077116  n^3 - A065733(n).
 * https://oeis.org/A077116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - floor(sqrt(n^3))^2 (0 when n is a perfect square)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* cast the index to seq_t for all arithmetic */
    seq_t sn  = (seq_t)n;
    seq_t cube = sn * sn * sn;        /* n^3 in seq_t */
    seq_t s    = floor_sqrt(cube);    /* floor(sqrt(n^3)) */
    return cube - s * s;              /* difference, 0 if n is a square */
}

/*
 * A214857  Number of triangular numbers in interval [0, n^2].
 * https://oeis.org/A214857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1 + sqrt(1+8*n^2))/2)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t sq = ns * ns;       /* n^2 */
    seq_t u  = sq * 8 + 1;    /* 1 + 8*n^2 */
    seq_t r  = floor_sqrt(u); /* floor sqrt of u */
    return (r + 1) / 2;       /* floor((1 + sqrt(u))/2) */
}

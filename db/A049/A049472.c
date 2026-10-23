/*
 * A049472  a(n) = floor(n/sqrt(2)).
 * https://oeis.org/A049472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 67, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    /* tmp = 2 * n * n, computed in seq_t to avoid overflow of idx_t */
    seq_t tmp = (seq_t)2 * nn * nn;
    seq_t sq = floor_sqrt(tmp);   /* floor(n * sqrt(2)) */
    return sq / 2;                /* floor((n * sqrt(2))/2) = floor(n/sqrt(2)) */
}

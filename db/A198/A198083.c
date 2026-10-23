/*
 * A198083  Ceiling(n*Sqrt(6)).
 * https://oeis.org/A198083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n * sqrt(6))
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t sq = (seq_t)n;
    sq *= sq;          /* n * n */
    sq *= (seq_t)6;    /* 6 * n * n */
    seq_t m = floor_sqrt(sq); /* floor(sqrt(6*n*n)) */
    return m + (seq_t)1;      /* ceil(n*sqrt(6)) */
}

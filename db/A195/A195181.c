/*
 * A195181  a(n) = 6*n - floor(3*n*sqrt(3)).
 * https://oeis.org/A195181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n - floor(3*n*sqrt(3))
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 6208, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the identity floor(3*n*sqrt(3)) = floor(sqrt(27*n*n)) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn;      /* n*n  */
    prod *= (seq_t)27;         /* 27*n*n */
    seq_t sq = floor_sqrt(prod);  /* floor(sqrt(27*n*n)) */
    return 6 * nn - sq;
}

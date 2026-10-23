/*
 * A195179  a(n) = 5*n - floor(2*n*sqrt(2)).
 * https://oeis.org/A195179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - floor(2*n*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* cast index to seq_t */
    seq_t prod = nn * nn;          /* n^2  */
    prod = prod * (seq_t)8;        /* 8*n^2  */
    seq_t b = floor_sqrt(prod);    /* floor(2*n*sqrt(2)) = floor(sqrt(8*n^2)) */
    return (seq_t)5 * nn - b;      /* 5*n - b  */
}

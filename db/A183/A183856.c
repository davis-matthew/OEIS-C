/*
 * A183856  n+floor(sqrt(3n-2)); complement of A143975.
 * https://oeis.org/A183856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(3*n-2))
 * Range:     u8 n <= 21, u16 n <= 5166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit integer, cast to seq_t for arithmetic */
    seq_t NN = (seq_t)n;
    seq_t t   = 3 * NN - (seq_t)2;        /* argument of the square root */
    seq_t sq  = floor_sqrt(t);            /* floor(sqrt(t)) */
    return NN + sq;                       /* a(n) = n + floor(sqrt(3n-2)) */
}

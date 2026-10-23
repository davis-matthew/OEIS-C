/*
 * A022849  Integer nearest nx, where x = sqrt(6).
 * https://oeis.org/A022849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n*sqrt(6)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;                 /* cast index to seq_t */
    seq_t sq  = (seq_t)6 * nn * nn;       /* 6*n^2       */
    seq_t s   = floor_sqrt(sq);           /* floor(√(6*n²)) */

    /* Determine if round up is needed:
       left  = 24*n²
       right = 4*s² + 4*s + 1
       If left ≥ right → ceil, else floor.      */
    seq_t left   = (seq_t)24 * nn * nn;
    seq_t right  = (seq_t)4 * s * s + (seq_t)4 * s + (seq_t)1;

    if (left >= right)
        return s + 1;
    else
        return s;
}

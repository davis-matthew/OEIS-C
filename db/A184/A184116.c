/*
 * A184116  a(n) = n + floor(5*sqrt(n + 1)).
 * https://oeis.org/A184116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(5 * sqrt(n+1))
 * Range:     u8 n <= 1, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor(5 * sqrt(n+1)) without floating point. */
    seq_t prod = (seq_t)25 * ((seq_t)n + 1);
    seq_t k = floor_sqrt(prod);   /* floor( sqrt(25 * (n+1)) ) = floor(5 * sqrt(n+1)) */
    return (seq_t)n + k;
}

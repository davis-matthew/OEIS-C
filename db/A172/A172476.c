/*
 * A172476  a(n) = floor(n/sqrt(6)).
 * https://oeis.org/A172476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n / sqrt(6))
 * Range:     u8 n <= 15, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_cast = (seq_t)n;
    seq_t n2     = n_cast * n_cast;   /* n² */
    seq_t s      = n2 / 6;            /* floor(n² / 6) */
    return floor_sqrt(s);            /* floor(sqrt(s)) = floor(n / sqrt(6)) */
}

/*
 * A071803  Number of paths in the lattice [0..n] X [0..n] X [0..n] which do
 *          not pass through the point (floor(n/2), floor(n/2), floor(n/2)).
 *          Number of paths through a lattice containing a "hole".
 * https://oeis.org/A071803
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = s(3,n) - s(3,floor(n/2))*s(3,ceil(n/2)) where s(3,n) =
 *            binom(3*n,n)*binom(2*n,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(2) is the first term of the sequence */
    idx_t half = n / 2;
    idx_t ceil = n - half;

    /* s(3,n) = binom(3*n,n) * binom(2*n,n) */
    seq_t s_n = binom(3*n, n);
    s_n = s_n * binom(2*n, n);

    /* s(3,floor(n/2)) */
    seq_t s_half = binom(3*half, half);
    s_half = s_half * binom(2*half, half);

    /* s(3,ceil(n/2)) */
    seq_t s_ceil = binom(3*ceil, ceil);
    s_ceil = s_ceil * binom(2*ceil, ceil);

    return s_n - (s_half * s_ceil);
}

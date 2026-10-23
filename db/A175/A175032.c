/*
 * A175032  a(n) is the difference between the n-th triangular number and the
 *          next perfect square.
 * https://oeis.org/A175032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = ceil(sqrt(n*(n+1)/2))^2 - n*(n+1)/2
 * Range:     u8 n <= 10, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute the n-th triangular number */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* floor of the square root of T */
    seq_t s = floor_sqrt(T);

    /* if s*s < T, increase s to obtain the ceiling */
    if (s * s < T)
        ++s;

    return s * s - T;
}

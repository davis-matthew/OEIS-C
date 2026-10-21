/*
 * A074704  a(n) = floor(n^(3/2)) - n*floor(n^(1/2)).
 * https://oeis.org/A074704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = floor(n^(3/2)) - n*floor(n^(1/2))
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 753, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow in multiplications */
    seq_t N = n;

    /* floor(sqrt(n)) */
    seq_t s = floor_sqrt(N);

    /* floor(n^(3/2)) = floor(sqrt(n^3)) = floor_sqrt(N*N*N) */
    seq_t n3 = N * N * N;
    seq_t t = floor_sqrt(n3);

    /* result = floor(n^(3/2)) - n*floor(sqrt(n)) */
    return t - N * s;
}

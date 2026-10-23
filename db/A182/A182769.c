/*
 * A182769  Beatty sequence for (4 + sqrt(2))/2.
 * https://oeis.org/A182769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(4 + sqrt(2))/2)
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic. */
    seq_t N = (seq_t)n;

    /* Compute t = 2 * N * N and its integer square root.
       floor(sqrt(2 * N * N)) equals floor(N * sqrt(2)). */
    seq_t t = 2 * N * N;
    seq_t s = floor_sqrt(t);

    /* The desired term: 2*N + floor(s/2). */
    return 2 * N + s / 2;
}

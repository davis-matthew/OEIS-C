/*
 * A194146  Beatty sequence for (4+sqrt(6))/2; complement of A194145.
 * https://oeis.org/A194146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(4 + sqrt(6))/2)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* We work with seq_t throughout to avoid overflow on large builds. */
    seq_t N = (seq_t)n;

    /* Compute floor(n * sqrt(6)) as floor(sqrt(6 * n^2)). */
    seq_t t = N * N;          /* n^2 */
    t = t * (seq_t)6;         /* 6 * n^2 */
    seq_t s = floor_sqrt(t);  /* floor(sqrt(6 * n^2)) */

    /* Now a(n) = floor((4n + s) / 2). */
    seq_t num = (N * (seq_t)4) + s;
    return num / (seq_t)2;
}

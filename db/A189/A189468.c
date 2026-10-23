/*
 * A189468  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=1+sqrt(2), t=1/2+sqrt(2).
 * https://oeis.org/A189468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/t⌋ + ⌊n*s/t⌋ where r = 1, s = 1+√2, t = 1/2+√2 The
 *            computation is performed using integer arithmetic only
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 5309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1, but guard anyway */

    seq_t N = (seq_t)n;

    /* floor(4*n*√2) = floor(√(32*n^2)) */
    seq_t floor_4nsqrt2 = floor_sqrt((seq_t)32 * N * N);

    /* floor(2*n*√2) = floor(√(8*n^2)) */
    seq_t floor_2nsqrt2 = floor_sqrt((seq_t)8 * N * N);

    /* floor((4n√2 – 2n)/7)  = (floor_4nsqrt2 – 2n) / 7   */
    seq_t termA = (floor_4nsqrt2 - (seq_t)2 * N) / (seq_t)7;

    /* floor((6n + 2n√2)/7)  = (6n + floor_2nsqrt2) / 7   */
    seq_t termB = ((seq_t)6 * N + floor_2nsqrt2) / (seq_t)7;

    return N + termA + termB;
}

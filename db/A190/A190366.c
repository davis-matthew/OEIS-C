/*
 * A190366  n + [n*r/t] + [n*s/t] + [n*u/t]; r=sqrt(2), s=1/r, t=sqrt(3),
 *          u=1/t.
 * https://oeis.org/A190366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2/3)) + floor(n*sqrt(1/6)) + floor(n/3)
 * Range:     u8 n <= 9, u16 n <= 629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"  /* provides seq_t floor_sqrt(seq_t x) */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    seq_t N = (seq_t)n;            /* convert index to seq_t */

    /* n + floor(n/3) */
    seq_t result = N + (N / 3);

    /* floor(n*sqrt(2/3)) = floor(sqrt(2*n^2 / 3)) */
    seq_t temp = (N * N * (seq_t)2) / (seq_t)3;
    result += floor_sqrt(temp);

    /* floor(n*sqrt(1/6)) = floor(sqrt(n^2 / 6)) */
    temp = (N * N) / (seq_t)6;
    result += floor_sqrt(temp);

    return result;
}

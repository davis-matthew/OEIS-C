/*
 * A195072  a(n) = n - floor(n/sqrt(3)).
 * https://oeis.org/A195072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n / sqrt(3))
 * Range:     u8 n <= 13, u16 n <= 1093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(n / sqrt(3)) as floor(sqrt(floor(n^2 / 3))) using integer arithmetic. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is >= 1 for the offset, but we handle n==0 gracefully. */
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;                 /* n^2 */
    seq_t div3 = sq / (seq_t)3;          /* floor(n^2 / 3)   */
    seq_t k = floor_sqrt(div3);         /* floor(sqrt( floor(n^2/3) )) = floor(n / sqrt(3)) */
    return sn - k;                      /* a(n) = n - floor(n / sqrt(3)) */
}

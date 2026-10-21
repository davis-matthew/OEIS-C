/*
 * A080755  a(n) = ceiling(n*(1+1/sqrt(2))).
 * https://oeis.org/A080755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*(1+1/sqrt(2))) = n + floor(n/√2) + 1
 * Range:     u8 n <= 11, u16 n <= 401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t n_seq = (seq_t)n;
    /* Compute floor(n/√2) as floor_sqrt(n^2 / 2) */
    seq_t sq = n_seq * n_seq;
    seq_t half_sq = sq / (seq_t)2;
    seq_t v = floor_sqrt(half_sq);
    /* a(n) = n + v + 1 */
    return n_seq + v + (seq_t)1;
}

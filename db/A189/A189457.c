/*
 * A189457  a(n) = n+[ns/r]+[nt/r]; r=2, s=sqrt(2), t=1+sqrt(2).
 * https://oeis.org/A189457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊ n·√2 / 2 ⌋ + ⌊ n·(1+√2) / 2 ⌋
 * Range:     u8 n <= 5, u16 n <= 84, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9902 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using identity: ⌊ n·√2 / 2 ⌋ = ⌊ ⌊ n·√2 ⌋ / 2 ⌋  */
/* and ⌊ n·(1+√2) / 2 ⌋ = ⌊ ( n + ⌊ n·√2 ⌋ ) / 2 ⌋
 * The value ⌊ n·√2 ⌋ is computed as ⌊ √( 2·n² ) ⌋ via integer sqrt.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard against n==0 */

    seq_t n_seq = (seq_t)n;
    seq_t sq = n_seq * n_seq;
    sq = sq * (seq_t)2;            /* sq = 2 * n² */

    seq_t floor_nsqrt2 = floor_sqrt(sq);   /* ⌊ n·√2 ⌋ */

    seq_t part1 = floor_nsqrt2 / (seq_t)2;
    seq_t part2 = (n_seq + floor_nsqrt2) / (seq_t)2;

    return n_seq + part1 + part2;
}

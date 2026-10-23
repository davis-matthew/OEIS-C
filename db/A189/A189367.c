/*
 * A189367  a(n) = n + [n*s/r] + [n*t/r]; r=2, s=sqrt(2), t=sqrt(3).
 * https://oeis.org/A189367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)/2) + floor(n*sqrt(3)/2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 3767, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t floor_sqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* floor(n * sqrt(2) / 2) = floor( sqrt(2 * n^2) ) / 2 */
    seq_t two_n_sq = nn * nn;
    two_n_sq *= 2;
    seq_t f2 = floor_sqrt(two_n_sq) >> 1;

    /* floor(n * sqrt(3) / 2) = floor( sqrt(3 * n^2) ) / 2 */
    seq_t three_n_sq = nn * nn;
    three_n_sq *= 3;
    seq_t f3 = floor_sqrt(three_n_sq) >> 1;

    return nn + f2 + f3;
}

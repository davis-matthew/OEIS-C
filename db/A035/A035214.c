/*
 * A035214  2 followed by a run of n 1's.
 * https://oeis.org/A035214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 if n is a triangular number, otherwise 1. The test uses
 *            the fact that n is triangular ⇔ 8*n+1 is a perfect square
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    seq_t val = ((seq_t)8 * (seq_t)n) + 1;   /* 8*n + 1 as seq_t */
    seq_t s   = isqrt_seq(val);
    seq_t sq2 = s * s;
    return (sq2 == val) ? (seq_t)2 : (seq_t)1;
}

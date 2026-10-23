/*
 * A165775  n + (least square >= n), i.e., n + A048761(n).
 * https://oeis.org/A165775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + ceil(sqrt(n))^2
 * Range:     u8 n <= 121, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) without overflow using binary search. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1, hi = n >> 1;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= n / mid) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    idx_t r = floor_sqrt_u64(n);
    /* Raise r to the next integer if n is not a perfect square. */
    if ((seq_t)r * (seq_t)r < (seq_t)n) {
        ++r;
    }
    seq_t sq = (seq_t)r * r;
    return (seq_t)n + sq;
}

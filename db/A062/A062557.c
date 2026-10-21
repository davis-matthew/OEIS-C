/*
 * A062557  2n-1 1's followed by a 2.
 * https://oeis.org/A062557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062557: a(n) = 1, except a(n) = 2 when
   n = k² + 3k + 1 for some integer k ≥ 0. */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for an unsigned seq_t. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = 1;
    /* Expand high until high*high > x, using division to avoid overflow. */
    while (high <= x / high) {
        high <<= 1;
    }
    /* Binary search between low (inclusive) and high (exclusive). */
    while (high - low > 1) {
        seq_t mid = low + (high - low) / 2;
        if (mid <= x / mid) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* d = 4*n + 5, we will test if d is a perfect square. */
    seq_t d = (seq_t)4 * (seq_t)n + (seq_t)5;
    seq_t s = isqrt_seq(d);
    if (s * s == d) {            /* perfect square */
        if (s >= (seq_t)3) {      /* s-3 must be non‑negative */
            seq_t diff = s - (seq_t)3;
            if ((diff & (seq_t)1) == 0)  /* (s-3) even => integer k exists */
                return (seq_t)2;
        }
    }
    return (seq_t)1;
}

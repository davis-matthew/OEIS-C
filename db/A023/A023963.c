/*
 * A023963  First digit after decimal point of 4th root of n.
 * https://oeis.org/A023963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10 * (n^(1/4) - floor(n^(1/4))))
 * Range:     u8 none, u16 n <= 6, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This can be computed as a(n) = floor(sqrt(sqrt(10000*n))) - 10*floor(sqrt(sqrt(n))) */

#include <stdint.h>
#include <stdbool.h>

static seq_t _isqrt_seq(seq_t x)
{
    /* Return ⌊√x⌋ using binary search */
    seq_t low = 0;
    seq_t high = 1;
    /* find high such that high*high > x, avoiding overflow */
    while (high <= x / high) {
        high <<= 1;
    }
    while (high - low > 1) {
        seq_t mid = low + (high - low) / 2;
        seq_t mid_sq = mid * mid;
        if (mid_sq <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t */
    seq_t sn = (seq_t)n;
    seq_t s10000n = (seq_t)10000 * sn;

    /* Compute floor fourth root of n */
    seq_t m = _isqrt_seq(_isqrt_seq(sn));

    /* Compute floor fourth root of 10000*n = 10*n^(1/4) */
    seq_t t = _isqrt_seq(_isqrt_seq(s10000n));

    /* First decimal digit after fourth root */
    return t - (seq_t)10 * m;
}

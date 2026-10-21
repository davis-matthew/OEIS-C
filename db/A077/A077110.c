/*
 * A077110  Nearest integer cube to n^2.
 * https://oeis.org/A077110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   nearest integer cube to n² a(n) = nearest cube to n². If tied,
 *            use the higher cube
 * Range:     u8 n <= 7, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer cube root floor for arbitrary-precision seq_t values. */
static seq_t cube_root_floor(seq_t v)
{
    seq_t lo = 0, hi = 1;
    /* find an upper bound hi such that hi³ > v */
    while (1) {
        seq_t hi3 = hi * hi * hi;
        if (hi3 > v)
            break;
        hi <<= 1;  /* double hi */
    }
    /* binary search between lo (<= root) and hi ( > root ) */
    while (lo + 1 < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t mid3 = mid * mid * mid;
        if (mid3 <= v)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t m = cube_root_floor(nsq);
    seq_t cube_low = m * m * m;
    seq_t m1 = m + 1;
    seq_t cube_high = m1 * m1 * m1;
    seq_t diff_low = nsq - cube_low;
    seq_t diff_high = cube_high - nsq;
    return (diff_low < diff_high) ? cube_low : cube_high;
}

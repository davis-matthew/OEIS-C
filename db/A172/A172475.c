/*
 * A172475  a(n) = floor(n*sqrt(3)/2).
 * https://oeis.org/A172475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(3)/2)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root: returns floor(sqrt(x)) for unsigned seq_t x. */
static seq_t _isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t result = 0;

    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t sq = mid * mid;

        if (sq == x) return mid;
        if (sq < x) {
            result = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t xx = m * m;          /* compute n*n without overflow if it fits seq_t */
    xx = xx * (seq_t)3;        /* multiply by 3  */
    seq_t s = _isqrt(xx);      /* floor(sqrt(3*n*n)) */
    return s / (seq_t)2;       /* floor( floor(sqrt(3*n*n)) / 2 ) equals floor(n*sqrt(3)/2) */
}

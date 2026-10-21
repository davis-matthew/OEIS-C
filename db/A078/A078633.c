/*
 * A078633  Smallest number of sticks of length 1 needed to construct n
 *          squares with sides of length 1.
 * https://oeis.org/A078633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n + ceil(2*sqrt(n))
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root of a 64‑bit integer. */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, s = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= (__int128)n) {
            s = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* index 0 is out of range, just return 0 */
    idx_t s = floor_isqrt_u64(n);       /* floor sqrt(n) */
    seq_t factor;
    if ((__int128)s * s == (__int128)n) {
        /* n is a perfect square, ceil(2*sqrt(n)) = 2*s */
        factor = (seq_t)2 * (seq_t)s;
    } else if ((seq_t)n <= (seq_t)s * s + s) {
        /* sqrt(n) < s + 0.5 -> ceil(2*sqrt(n)) = 2*s + 1 */
        factor = (seq_t)2 * (seq_t)s + 1;
    } else {
        /* sqrt(n) >= s + 0.5 -> ceil(2*sqrt(n)) = 2*s + 2 */
        factor = (seq_t)2 * (seq_t)s + 2;
    }
    return 2 * (seq_t)n + factor;
}

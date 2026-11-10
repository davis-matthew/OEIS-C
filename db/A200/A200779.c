/*
 * A200779  a(n) = number of i in the range 1 <= i <= n such that b(i)=b(n),
 *          where b is the sequence A053615 taken with offset 1.
 * https://oeis.org/A200779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 0 <= m <= n-1 : |m - t(t+1)| = v, t = floor(sqrt(m))
 *            }, where v = A053615(n-1); counted in closed form by blocks of
 *            squares
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A200779_fsqrt(idx_t n)
{
    if (n == 0) return 0;
    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= n / mid) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

static idx_t A200779_dist(idx_t m)
{
    idx_t t = A200779_fsqrt(m);
    idx_t p = t * (t + 1);
    return m >= p ? m - p : p - m;
}

seq_t a(idx_t n)
{
    idx_t M = n - 1;
    idx_t v = A200779_dist(M);
    idx_t T = A200779_fsqrt(M);
    idx_t cnt = 0;
    if (v == 0) {
        cnt = T;
    } else if (T > v) {
        cnt = 2 * (T - v);
    }
    if (v <= T) {
        idx_t p = T * (T + 1);
        if (p - v <= M) cnt++;
        if (v > 0 && p + v <= M) cnt++;
    }
    return (seq_t)cnt;
}

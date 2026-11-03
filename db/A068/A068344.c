/*
 * A068344  Square array read by antidiagonals of T(n,k) = sign(n-k).
 * https://oeis.org/A068344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = sign(2 * r - t) where * t = the largest integer with
 *            t(t+1)/2 <= n * r = n - t(t+1)/2, 0 <= r <= t * * The function
 *            uses only integer arithmetic
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Validated: partial, pass, 102 terms through n = 101 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_t(idx_t n)
{
    /* Find t by doubling then binary search.  The condition
     *   t(t+1)/2 <= n
     * is monotonic in t. */
    idx_t lo = 0, hi = 1;
    while ((( __int128)hi * (hi + 1)) / 2 <= n) {
        lo = hi;
        hi <<= 1;
    }
    while (hi - lo > 1) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((( __int128)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t t = find_t(n);                     /* t(t+1)/2 <= n < (t+1)(t+2)/2 */
    idx_t f = t * (t + 1) / 2;               /* f = t(t+1)/2 */
    idx_t r = n - f;                         /* offset within antidiagonal */
    seq_t diff = (seq_t)(r << 1) - (seq_t)t; /* 2*r - t */
    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;
}

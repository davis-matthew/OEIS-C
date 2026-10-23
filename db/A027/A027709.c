/*
 * A027709  Minimal perimeter of polyomino with n square cells.
 * https://oeis.org/A027709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*ceiling(2*sqrt(n)) (a(0)=0)
 * Range:     u8 n <= 4032, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


/* Compute floor integer square root of an unsigned 64‑bit integer. */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, best = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            best = 0;
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* floor(sqrt(n)) */
    idx_t r = _floor_isqrt_u64(n);
    idx_t r2 = r * r; /* r squared */

    idx_t c; /* ceil(2*sqrt(n)) */

    if (r2 == n) {
        /* n is a perfect square */
        c = 2 * r;
    } else if (n <= r2 + r) {
        /* sqrt(n) < r + 0.5 */
        c = 2 * r + 1;
    } else {
        /* sqrt(n) >= r + 0.5 */
        c = 2 * r + 2;
    }

    /* a(n) = 2 * c */
    return (seq_t)(2 * c);
}

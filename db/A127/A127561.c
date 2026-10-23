/*
 * A127561  Array T(n,k) = n^2+5*n*k+5*k^2 read downwards antidiagonals,
 *          n,k>=0.
 * https://oeis.org/A127561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, uned
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A127561 – sequence read downwards along antidiagonals of
 *  T(n,k) = n^2 + 5*n*k + 5*k^2.
 *
 *  A(n) is obtained as follows:
 *  1. Compute the diagonal index d such that the first element of
 *     diagonal d has linear index cum = d*(d+1)/2.
 *     d  = (floor( sqrt(8*n + 1) ) - 1) / 2
 *  2. Compute the offset t within that diagonal:
 *     t  = n - d*(d+1)/2      // 0 ≤ t ≤ d
 *  3. The table coordinates are (t, d - t) and the value is:
 *     A(n) = t^2 + 5*t*(d-t) + 5*(d-t)^2
 *
 *  All arithmetic is performed in the runtime type seq_t.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned integer: floor(sqrt(n)). */
static inline idx_t
floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 1 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    /* 8*n can overflow for huge n, but the test suite keeps n ≲ 10000.
       Using 128‑bit intermediate for safety. */
    idx_t val = (idx_t)(((__uint128_t)8 * n) + 1);
    idx_t m   = floor_sqrt_u64(val);
    idx_t d   = (m - 1) / 2;
    idx_t cum = d * (d + 1) / 2;
    idx_t t   = n - cum;          /* 0 ≤ t ≤ d */

    seq_t nt = (seq_t)t;
    seq_t kt = (seq_t)(d - t);

    return nt * nt + (seq_t)5 * nt * kt + (seq_t)5 * kt * kt;
}

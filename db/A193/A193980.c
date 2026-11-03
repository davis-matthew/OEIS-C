/*
 * A193980  Mirror of the triangle A193979.
 * https://oeis.org/A193980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((k^2 + k + 2)/2) * r + (-k^3 + 3k^2 - 2k + 6)/6 * where
 *            r = floor((sqrt(8*n + 1) - 1) / 2) and k = r - (n - r*(r+1)/2)
 * Range:     u8 n <= 44, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute w(r, k) from the triangle A193979. */
static inline seq_t
w_triangle(idx_t r, idx_t k)
{
    /* Use signed 128‑bit for intermediate calculations to allow
       possible negative values of the b_k term. */
    __int128 kk = (int64_t)k;
    __int128 rk = (int64_t)r;
    /* a_k = (k^2 + k + 2) / 2 */
    __int128 ak = (kk * kk + kk + 2) / 2;
    /* b_k = (-k^3 + 3k^2 - 2k + 6) / 6 */
    __int128 bk = (-kk * kk * kk + 3 * kk * kk - 2 * kk + 6) / 6;
    __int128 val = ak * rk + bk;
    return (seq_t)val;
}

/* Integer square root for 64‑bit values (floor). */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;          /* row index */
    idx_t start = r * (r + 1) / 2;                     /* index of first term of row r */
    idx_t local = n - start;                           /* position within the row */
    idx_t k = r - local;                               /* mirrored column index */
    return w_triangle(r, k);
}

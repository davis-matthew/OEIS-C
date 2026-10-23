/*
 * A049581  Table T(n,k) = |n-k| read by antidiagonals (n >= 0, k >= 0).
 * https://oeis.org/A049581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   for n >= 0, * let d = floor((sqrt(1 + 8*n) - 1)/2) (the diagonal
 *            index) * let s = d*(d+1)/2 (starting index of that diagonal) *
 *            let p = n - s (position within the diagonal) * then a(n) = |d -
 *            2*p|
 * Range:     u8 n <= 8000, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit value, returning the floor of sqrt(x). */
static inline idx_t isqrt_u128(__int128 x)
{
    idx_t lo = 0, hi = (idx_t)(((__int128)1 << 64) - 1);
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Compute d = floor((sqrt(1 + 8*n) - 1)/2) */
    __int128 val = (__int128)1 + (__int128)8 * n;
    idx_t s    = isqrt_u128(val);          /* floor sqrt(1+8n) */
    idx_t d    = (s - 1) >> 1;              /* integer division by 2 */

    /* Compute start index of diagonal d: s_d = d*(d+1)/2 */
    idx_t start = (idx_t)(((__int128)d * (d + 1)) >> 1); /* divide by 2 */

    /* Position within the diagonal */
    idx_t pos = n - start;

    /* a(n) = |d - 2*pos| */
    seq_t left  = (seq_t)d;
    seq_t right = (seq_t)(pos << 1);         /* 2*pos */

    return (left > right) ? (left - right) : (right - left);
}

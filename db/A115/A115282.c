/*
 * A115282  Correlation triangle for the sequence 3-2*0^n.
 * https://oeis.org/A115282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = ⌊(√(8 n + 1) − 1)/2⌋ (row index) * k =
 *            n − r(r + 1)/2 (column index) * * T(r,k) = 9·min(k,r−k) + 3 *
 *            except when r even and k = r/2, then T=9·k + 1. * * This is the
 *            correlation triangle for the sequence 3−2·0ⁿ
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root (floor) for 64‑bit unsigned integers,
   implemented with binary search and division to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* (2^32)^2 > 2^64 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1; /* upper mid */
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8*n+1) - 1)/2) */
    idx_t val = 8 * n + 1;
    idx_t sq  = isqrt_u64(val);
    idx_t r   = (sq - 1) >> 1;          /* r = (sq-1)/2 */

    /* k = n - r(r+1)/2 */
    idx_t offset = r * (r + 1) >> 1;    /* r*(r+1)/2 */
    idx_t k = n - offset;

    /* Evaluate T(r,k) */
    idx_t other = r - k;                /* r-k */
    idx_t m = (k < other) ? k : other; /* min(k,r-k) */

    if ((r & 1) == 0 && k == other) {  /* r even and k==r/2 */
        return (seq_t)(9ULL * (seq_t)m + 1ULL);
    } else {
        return (seq_t)(9ULL * (seq_t)m + 3ULL);
    }
}

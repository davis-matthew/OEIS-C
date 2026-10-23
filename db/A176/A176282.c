/*
 * A176282  Triangle T(n,k) = 1 + A000330(n) - A000330(k) - A000330(n-k), read
 *          by rows.
 * https://oeis.org/A176282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 + k*(r+1)*(r-k) where * r = floor((sqrt(8*n+1)-1)/2)
 *            (row index) * k = n - r*(r+1)/2 (column index within that row)
 * Range:     u8 n <= 58, u16 n <= 2108, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor), works for all 64-bit values. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32; /* sqrt(UINT64_MAX) < 2^32  */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= x / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Compute row index r.  r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t t  = 8 * n + 1;                /* 8n+1 fits 64‑bit for all n used */
    idx_t sq = isqrt_u64(t);
    idx_t r  = (sq - 1) / 2;             /* floor((sqrt(8n+1)-1)/2) */

    idx_t s = r * (r + 1) / 2;           /* first index of row r */
    idx_t k = n - s;                     /* column within the row */

    /* Compute the triangle value using seq_t arithmetic. */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    return 1 + sk * (sr + 1) * (sr - sk);
}

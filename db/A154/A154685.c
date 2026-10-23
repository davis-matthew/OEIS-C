/*
 * A154685  Triangle read by rows: T(n, k) = 2*n*k + n + k + 4.
 * https://oeis.org/A154685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*r*k + r + k + 4 * where r = ceil((sqrt(8*n+1)-1)/2) is
 *            the row of the triangle, * and k = n - r*(r-1)/2 is the column
 *            position within that row
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for 64‑bit values */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* compute r: the row number */
    __int128 d128 = (__int128)8 * n + 1;
    idx_t D = (idx_t)d128;                    /* safe for typical n */
    idx_t sqrtD = isqrt_u64(D);
    idx_t r = (sqrtD - 1) / 2;
    if (r * (r + 1) / 2 < n) r++;              /* ensure ceil */
    /* compute k: column within row */
    idx_t k = n - r * (r - 1) / 2;
    /* convert to seq_t and evaluate the formula */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    return 2 * sr * sk + sr + sk + 4;
}

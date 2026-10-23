/*
 * A131217  Triangle read by rows: T(n, k) = A047999(n, k) + (n+1 mod
 *          2)*[k=1].
 * https://oeis.org/A131217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) =
 *            A047999(n,k) + (n+1 mod 2)*[k==1] (Pascal’s triangle mod 2 with
 *            column 1 forced to 1). For n≥0 let r = ⌊(√(1+8n)−1)/2⌋ // row
 *            index k = n − r(r+1)/2 // column index within that row Then a(n)
 *            = 1 if k==1 1 if (k & ~r) == 0 // Pascal(n,k) odd 0 o...
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u128(__int128 x)
{
    idx_t lo = 0, hi = ((idx_t)1) << 63, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* compute r = floor((sqrt(1+8n)-1)/2) */
    __int128 v = (__int128)1 + (__int128)8 * n;
    idx_t sqrtv = isqrt_u128(v);
    idx_t r = (sqrtv - 1) / 2;

    /* column index k within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* evaluate T(r,k) */
    seq_t val = 0;
    if (k == 1) {
        val = 1;
    } else if ((k & ~r) == 0) {  /* Pascal mod 2 == 1 */
        val = 1;
    }

    return val;
}

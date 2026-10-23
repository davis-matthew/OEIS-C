/*
 * A119502  Triangle read by rows, T(n,k) = (n-k)!, for n>=0 and 0<=k<=n.
 * https://oeis.org/A119502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (⌊(√(8n+1)-1)/2⌋ - (n -
 *            ⌊(√(8n+1)-1)/2⌋(⌊(√(8n+1)-1)/2⌋+1)/2))!
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root by binary search (no overflow) */
static inline idx_t isqrt_idx(idx_t v)
{
    if (v == 0) return 0;
    idx_t lo = 1, hi = v, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= v / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8n+1)-1)/2)  */
    idx_t temp = 1 + (idx_t)8 * n;
    idx_t s    = isqrt_idx(temp);
    idx_t r    = (s - 1) / 2;

    /* offset of the first element of row r */
    idx_t offset;
    if (r & 1) /* r odd */
        offset = r * ((r + 1) / 2);
    else       /* r even */
        offset = (r / 2) * (r + 1);

    /* column index k = n - offset, factorial argument m = r - k */
    idx_t k = n - offset;
    idx_t m = r - k;

    /* compute m! */
    seq_t res = 1;
    for (idx_t i = 2; i <= m; ++i)
        res *= (seq_t)i;          /* multiplication works for all seq_t types */

    return res;
}

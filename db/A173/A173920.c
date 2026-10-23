/*
 * A173920  Triangle read by rows: T(n,k) = convolution of n with k in binary
 *          representation, 0<=k<=n.
 * https://oeis.org/A173920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = sum_{i=0}^{L-1} ((r>>i)&1) &
 *            ((k>>(L-1-i))&1) with L = max(1, floor_log2_uint64(r)+1)
 * Range:     u8 n <= 9708, u16 n <= 9708, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

static idx_t tri_count(idx_t r)
{
    /* returns r*(r+1)/2 using 128‑bit arithmetic to avoid overflow */
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

static seq_t tri_convolution(idx_t r, idx_t k)
{
    idx_t L;
    if (r == 0)
        L = 1;
    else
        L = (unsigned)floor_log2_uint64(r) + 1;

    seq_t sum = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        if ( ((r >> i) & 1ULL) && ((k >> (L - 1 - i)) & 1ULL) )
            ++sum;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t lo = 0;
    idx_t hi = 2;
    while (tri_count(hi) <= n)
        hi <<= 1;                   /* exponential search for upper bound */

    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (tri_count(mid) > n)
            hi = mid;
        else
            lo = mid + 1;
    }

    idx_t r = lo - 1;
    idx_t cum_before = tri_count(r);
    idx_t k = n - cum_before;

    return tri_convolution(r, k);
}

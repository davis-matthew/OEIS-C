/*
 * A204012  Symmetric matrix based on f(i,j)=min{3i+j-3,i+3j-3}, by
 *          antidiagonals.
 * https://oeis.org/A204012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = min{3*i + j - 3, i + 3*j - 3} for the * (i,j) obtained by
 *            writing the matrix * * M(i,j) = min{3*i + j - 3, i + 3*j - 3}
 *            (i,j≥1) * * along its antidiagonals: each diagonal d = i+j-1
 *            contains d * entries. For a given index n≥1, find the smallest d
 *            such that * d(d+1)/2 ≥ n. Let pos = n - (d-1...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt_uint128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)~0;
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
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
    if (n < 1) return 0;               /* safety for n=0, though offset is 1 */

    /* Find diagonal d such that d(d+1)/2 ≥ n */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t s = floor_sqrt_uint128(tmp);
    idx_t d = (s - 1) / 2;             /* floor estimate */
    /* adjust up or down to satisfy the inequality */
    while (((unsigned __int128)d * (d + 1) / 2) < n) ++d;
    while (d > 0 && (((unsigned __int128)(d - 1) * d) / 2) >= n) --d;

    /* offset within diagonal */
    unsigned __int128 prev_total = (unsigned __int128)(d - 1) * d / 2;
    idx_t pos = (idx_t)(n - prev_total);   /* 1 ≤ pos ≤ d */

    idx_t i = pos;
    idx_t j = d - pos + 1;

    seq_t val1 = (seq_t)3 * (seq_t)i + (seq_t)j - (seq_t)3;
    seq_t val2 = (seq_t)i + (seq_t)3 * (seq_t)j - (seq_t)3;
    return val1 < val2 ? val1 : val2;
}

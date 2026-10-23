/*
 * A161381  Triangle read by rows: T(n,k) = n!*2^k/(n-k)! (n >= 0, 0 <= k <=
 *          n).
 * https://oeis.org/A161381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2, and
 *            T(r,k)=r!*2^k/(r-k)! (triangle read by rows)
 * Range:     u8 n <= 13, u16 n <= 32, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r: largest r such that r*(r+1)/2 <= n. */
    idx_t low = 0;
    idx_t high = n;                 /* r never exceeds n */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        /* use 128‑bit arithmetic to avoid overflow */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column k */
    idx_t offset = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Compute T(r,k) = (r!/(r-k)!) * 2^k */
    seq_t res = 1;
    for (idx_t i = r - k + 1; i <= r; ++i)
        res *= (seq_t)i;
    res *= pow2(k);
    return res;
}

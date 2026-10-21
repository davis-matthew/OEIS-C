/*
 * A051731  Triangle read by rows: T(n, k) = 1 if k divides n, T(n, k) = 0
 *          otherwise, for 1 <= k <= n.
 * https://oeis.org/A051731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl, changed
 * Formula:   a(n) = 1 if the column index k divides the row index r of the
 *            (r,k)-entry of the lower‑triangular 1/0 triangle where T(r,k)=1
 *            ⇔ k|r, and 0 otherwise. The index n is interpreted as the
 *            row‑major flattening of that triangle. For a given n, r =
 *            smallest integer with r*(r+1)/2 ≥ n k = n - r*(r-1)/2 T...
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute row r such that r*(r+1)/2 >= n
       using binary search on 64‑bit integers. */
    uint64_t lo = 1, hi = n;          // hi can be n for n>=1
    while (lo < hi) {
        uint64_t mid = (lo + hi) / 2;
        __int128 cum = (__int128)mid * (mid + 1) / 2;
        if (cum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t r = lo;
    /* Compute column index k */
    uint64_t before = r * (r - 1) / 2;   // cumulative terms before row r
    uint64_t k = n - before;             // 1‑based column index

    /* Return 1 if k divides r, else 0 */
    return (seq_t)(r % k == 0 ? 1 : 0);
}

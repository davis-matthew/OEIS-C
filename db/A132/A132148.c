/*
 * A132148  Triangular array T(n,k) = C(n,k)*Lucas(n-k), 0 <= k <= n.
 * https://oeis.org/A132148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(r, k) * Lucas(r-k) where r = floor((sqrt(8n+1)-1)/2)
 *            (row number), k = n - r(r+1)/2 (column index)
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Iterative computation of the Lucas numbers.  Lucas(0)=2, Lucas(1)=1. */
static seq_t lucas(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)1;
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur   = prev1 + prev2;  /* Lucas(n) = Lucas(n-1) + Lucas(n-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    __int128 tri = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;             /* column index in row r */

    seq_t bin = binom(r, k);              /* C(r, k) */
    seq_t luc = lucas(r - k);             /* Lucas(r-k) */

    return bin * luc;
}

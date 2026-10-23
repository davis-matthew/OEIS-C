/*
 * A147861  Triangle read by rows: T(n,k)=min(k, n/k) if k divides n, T(n,k)=0
 *          otherwise (n >=1, 1<=k<=n).
 * https://oeis.org/A147861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row number with * 1 + 2 + ... +
 *            (r-1) < n ≤ 1 + 2 + ... + r, * k = n - (r-1)·r/2, * T(r,k) =
 *            min(k, r/k) if k divides r, otherwise 0
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for a triangle read by rows.  */

#include <stdint.h>

/* returns k = n - T(r-1), 0‑based row and column with 1‑based counting.   */
static inline unsigned __int128
triangular(idx_t r)
{
    return (unsigned __int128)r * (r + 1) / 2;
}

/* finds row r such that triangular(r-1) < n ≤ triangular(r)      */
static idx_t
row_of(idx_t n)
{
    idx_t low  = 1, high = 1;

    /* find an upper bound for high */
    while (triangular(high) < n) {
        high <<= 1;
    }

    /* binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t r = row_of(n);
    /* k = n - T(r-1) */
    idx_t k = n - (idx_t)triangular(r - 1);

    /* check divisibility */
    if (k == 0 || r % k != 0) {
        return (seq_t)0;
    }

    seq_t kk = (seq_t)k;
    seq_t rr = (seq_t)r;
    seq_t vk = rr / kk;          /* r / k */
    return kk < vk ? kk : vk;    /* min(k, r/k) */
}

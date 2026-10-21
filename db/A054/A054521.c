/*
 * A054521  Triangle read by rows: T(n,k) = 1 if gcd(n, k) = 1, T(n,k) = 0
 *          otherwise (n >= 1, 1 <= k <= n).
 * https://oeis.org/A054521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if gcd(row, col)=1 else 0, where (row,col) are obtained
 *            by reading the triangle T(n,k)=1_{gcd(n,k)=1} row‑by‑row.
 *            Implements the sequence A054521
 * Range:     u8 n <= 44, u16 n <= 4586, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = n; /* high can be n; binary search is O(log n) */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        seq_t t = (seq_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low; /* row index */

    /* Column index k within this row */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;

    /* Return 1 if r and k are coprime, 0 otherwise */
    seq_t g = gcd((seq_t)r, (seq_t)k);
    return (g == 1) ? 1 : 0;
}

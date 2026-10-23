/*
 * A103218  Triangle read by rows: T(n, k) = (2*k+1)*(n+1-k)^2.
 * https://oeis.org/A103218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*k+1)*(r+1-k)^2, where r = ⌊(√(8*n+1)-1)/2⌋ (the row
 *            index), k = n - r*(r+1)/2 (column index within that row)
 * Range:     u8 n <= 46, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n.
       Binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= ( __int128 )n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid underflow when mid==0 */
            high = mid - 1;
        }
    }

    /* Column index k within row r */
    idx_t k = n - (idx_t)((__int128)r * (r + 1) / 2);

    /* Compute (2*k+1) * (r+1-k)^2 using seq_t types. */
    seq_t mult = (seq_t)2 * (seq_t)k + (seq_t)1;
    seq_t base = (seq_t)(r + 1 - k);   /* r+1-k is always nonnegative */
    return mult * base * base;
}

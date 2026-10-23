/*
 * A120885  Triangle read by rows where t(n,m) = ceiling(n/m).
 * https://oeis.org/A120885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ceiling(r / c) where r is the row index such that
 *            r(r+1)/2 >= n > r(r-1)/2 and c = n - r(r-1)/2 (column index)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Binary search to find the row r such that r(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Use 128‑bit arithmetic to avoid overflow in the comparison */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute the cumulative count up to the previous row */
    __int128 prev_sum = (__int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_sum;          /* column index */

    /* Ceiling of r / c: (r + c - 1) / c */
    seq_t val = (seq_t)((seq_t)r + (seq_t)c - 1) / (seq_t)c;
    return val;
}

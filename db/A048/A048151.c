/*
 * A048151  Triangular array T read by rows: T(n,k)=k mod n, for k=1,2,...,n,
 *          n=1,2,...
 * https://oeis.org/A048151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ( (n - (r-1)*r/2) + 1 ) mod r where r = smallest integer
 *            with r*(r+1)/2 >= n (i.e., the row index of the triangular
 *            array)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* find r via binary search */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                      /* row number */
    __int128 prev_tri = (__int128)r * (r - 1) / 2;
    idx_t prev = (idx_t)prev_tri;        /* sum of rows before r */
    idx_t offset = n - prev - 1;        /* 0-based position in row r */
    idx_t k = offset + 1;               /* column index (1..r) */
    return (seq_t)(k % r);              /* T(r,k) = k mod r */
}

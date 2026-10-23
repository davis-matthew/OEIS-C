/*
 * A128140  A128132 * A004736.
 * https://oeis.org/A128140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r-1)*(r-k+1)+1 where r = min{ m | m(m+1)/2 >= n } (row
 *            number) k = n - (r-1)r/2 (column within that row) This computes
 *            the nth entry of the triangle with rows 1; 3,2; 7,5,3;
 *            13,10,7,4;
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find smallest r such that r(r+1)/2 >= n */
    idx_t low = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit arithmetic to avoid overflow */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* position within the row (1‑based) */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    /* compute the value */
    seq_t val = (seq_t)(r - 1) * (seq_t)(r - k + 1) + (seq_t)1;
    return val;
}

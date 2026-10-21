/*
 * A220075  Partial sums in rows of A220073, triangle read by rows.
 * https://oeis.org/A220075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{i=1}^{k} F(r,i) where r = smallest integer with
 *            r(r+1)/2 >= n k = n - (r-1)r/2 and F(r,i) = r (i == r) = r+1-2*i
 *            (i <= r/2) = 2*i-r (otherwise)
 * Range:     u8 n <= 275, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    /* number of terms before row r */
    uint64_t row_start = (uint64_t)(r - 1) * r / 2;
    idx_t k = n - row_start;     /* 1 <= k <= r */

    seq_t sum = 0;
    idx_t half = r >> 1;          /* floor(r/2) */

    for (idx_t i = 1; i <= k; ++i) {
        if (i == r) {
            sum += (seq_t)r;      /* last element is r */
        } else if (i <= half) {
            sum += (seq_t)(r + 1ULL - 2ULL * i);
        } else {
            sum += (seq_t)(2ULL * i - r);
        }
    }
    return sum;
}

/*
 * A174028  Triangle T(n,k) = 2+4k read by rows.
 * https://oeis.org/A174028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2 + 4 * ( n - r*(r+1)/2 ) * where r = floor((sqrt(8*n +
 *            1) - 1)/2) * i.e., r is the largest integer with r*(r+1)/2 ≤ n
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;                     /* r ≤ n */
    while (low < high) {
        /* Upper mid to avoid infinite loop when low+1==high */
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    seq_t prefix = (seq_t)r * ((seq_t)r + 1) / 2;
    seq_t k = (seq_t)n - prefix;
    return (seq_t)2 + (seq_t)4 * k;
}

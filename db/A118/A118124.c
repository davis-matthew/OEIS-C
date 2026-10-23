/*
 * A118124  Triangle T(n,m) = (n+m)^2+n+m+41, read by rows.
 * https://oeis.org/A118124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, tabl
 * Formula:   a(n) = (r+m)^2 + r + m + 41, * where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index), * m = n - r*(r+1)/2
 *            (column index within the row)
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    /* Column index within the row */
    idx_t m = n - (r * (r + 1)) / 2;

    /* Compute (r+m)^2 + r + m + 41 */
    seq_t sum = (seq_t)(r + m);
    return sum * sum + sum + (seq_t)41;
}

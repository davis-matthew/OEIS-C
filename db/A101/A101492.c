/*
 * A101492  Triangle read by rows: T(n,k) = (n-k+1)*(4*k+1).
 * https://oeis.org/A101492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows for T(r,k) = (r-k+1)*(4*k+1)
 * Range:     u8 n <= 126, u16 n <= 9966, u32 n <= 9966, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t total = 0;   /* number of terms before the current row */
    idx_t r = 0;       /* current row index */

    /* find the row r such that total + (r+1) > n */
    while (1) {
        idx_t count = r + 1;                 /* elements in row r */
        if (total + count > n) {
            break;
        }
        total += count;
        r++;
    }

    idx_t k = n - total;                     /* offset within row r */

    /* compute T(r,k) = (r - k + 1) * (4*k + 1) */
    seq_t part1 = (seq_t)(r - k + 1);
    seq_t part2 = ((seq_t)4 * (seq_t)k) + 1;
    return part1 * part2;
}

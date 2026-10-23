/*
 * A134482  Triangle read by rows: row n consists of n followed by the numbers
 *          n through 2n-2.
 * https://oeis.org/A134482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r + max(0, n - (r*(r-1)/2 + 1) - 1) with r =
 *            ⌊(√(8(n-1)+1)+1)/2⌋ (row index of the triangle)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard for non‑positive indices */

    /* Binary search for the row r: the largest r such that
     * first_index_of_row(r) = r*(r-1)/2 + 1 <= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);   /* mid = ceil((low+high)/2) */
        idx_t first = (mid * (mid - 1)) / 2 + 1;     /* first index of row mid */
        if (first <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t first = (r * (r - 1)) / 2 + 1;
    idx_t pos  = n - first;                         /* position within row (0‑based) */

    if (pos == 0)   /* first element of the row */
        return (seq_t)r;
    else            /* subsequent elements */
        return (seq_t)r + (seq_t)(pos - 1);
}

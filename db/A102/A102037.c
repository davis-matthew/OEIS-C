/*
 * A102037  Triangle of BitAnd(BitNot(n), k).
 * https://oeis.org/A102037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (bitwise AND of bitwise NOT of r with (n - r*(r+1)/2)),
 *            where r is the unique non‑negative integer with r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 (the row index of the triangular array)
 * Range:     u8 n <= 44, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row index r: r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = n;                     /* upper bound works for any n */
    while (low < high) {
        /* mid = ceil((low+high)/2) to avoid infinite loop */
        idx_t mid = low + ((high - low + 1) >> 1);
        seq_t t = (seq_t)mid * ((seq_t)mid + 1) / 2;   /* mid*(mid+1)/2 */
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Starting index of row r: T(r) = r*(r+1)/2 */
    seq_t start = (seq_t)r * ((seq_t)r + 1) / 2;

    /* Column offset within the row */
    idx_t k = n - (idx_t)start;

    /* Compute the triangle value: k AND (bitwise NOT of r) */
    return ((seq_t)k) & (~((seq_t)r));
}

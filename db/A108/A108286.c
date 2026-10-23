/*
 * A108286  Triangle read by rows; columns are simple recursive sequences.
 * https://oeis.org/A108286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = value of column c at row r where
 * r is minimal with r(r+1)/2 ≥ n,
 * c = n - r(r-1)/2.
 * For a fixed column c the sequence is defined by
 * b1 = 1,  bk = c*b(k‑1) + k   (k ≥ 2).   */
/* The function works for any integer types seq_t and idx_t. */
#include <stdbool.h>   /* only for the compliance requirement */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n and (r-1)r/2 < n.  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }
    /* Number of entries before this row.  */
    idx_t before = (r * (r - 1)) / 2;
    /* Column index c (1‑based).  */
    idx_t c = n - before;
    /* Local index in the column: the k‑th element of that column.  */
    idx_t k = r - c + 1;

    seq_t val = 1;            /* b1 = 1  */
    for (idx_t i = 2; i <= k; ++i) {
        val = (seq_t)c * val + i;
    }
    return val;
}

/*
 * A131818  A130296 + A002260 - A000012. Triangle read by rows: row n consists
 *          of n, 2, 3, 4, ..., n.
 * https://oeis.org/A131818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is defined by the lower‑triangular matrix
   A130296 + A002260 - A000012.
   Row n consists of the numbers n, 2, 3, …, n.
   The sequence a(n) follows this triangle read by rows
   (first row 1; second row 2,2; third row 3,2,3; …).
   For a given index n (1‑based) the value is:
      * if n is the first element of its row → the row number,
      * otherwise the column index of that element. 
   The code below finds the row containing n by binary search
   and then returns the appropriate value. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the smallest r such that r*(r+1)/2 >= n.
       Binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low  = 1;
    idx_t high = 1;

    /* Expand high until the cumulative count exceeds n. */
    while (((unsigned __int128)high * (high + 1)) / 2 < n) {
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 cum = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (cum >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    idx_t r = low; /* row number */

    /* Offset of the first element of row r (number of elements before this row). */
    idx_t offset = (idx_t)(((unsigned __int128)(r - 1) * r) / 2);

    /* Position inside the row (1‑based). */
    idx_t pos = n - offset; /* 1 ≤ pos ≤ r */

    /* Return the value: first position → row number, else → column index. */
    return (pos == 1) ? (seq_t)r : (seq_t)pos;
}

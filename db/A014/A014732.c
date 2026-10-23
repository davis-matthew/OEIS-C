/*
 * A014732  Squares of numbers in triangle of Eulerian numbers that are not 1.
 * https://oeis.org/A014732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = A014449(n)^2 (square of the nth interior Eulerian number)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   free_seq, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq.h"

/* Find minimal row r (r >= 3) such that the total number of interior
   Eulerian entries up to that row is >= n.  */
static idx_t
find_row(idx_t n)
{
    /* We need the smallest r with (r-2)(r-1)/2 >= n.  */
    idx_t low = 3, high = 3;
    while ((__int128)(high - 2) * (high - 1) / 2 < n)
        high <<= 1;
    low = high >> 1;               /* low < high and satisfies condition */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)(mid - 2) * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Row index containing the nth interior Eulerian number.  */
    idx_t row = find_row(n);

    /* Number of interior entries before this row.  */
    idx_t prev_total = (row - 3) * (row - 2) / 2;

    /* 0‑based offset within the interior of this row.  */
    idx_t offset = n - prev_total - 1;   /* 0‑based */
    idx_t m = offset + 1;                /* Eulerian column index (1‑based) */

    /* Allocate arrays for building rows.  */
    seq_t *prev = seq_alloc_new_array(row);
    seq_t *cur  = seq_alloc_new_array(row);
    if (!prev || !cur) {
        free_seq(prev);
        free_seq(cur);
        return (seq_t)0; /* allocation failed – value undefined */
    }

    /* Row 1 and row 2 initial values (avoid accessing out of bounds).  */
    prev[0] = (seq_t)1;
    if (row > 1)
        prev[1] = (seq_t)1;

    /* Build rows iteratively up to the desired row.  */
    for (idx_t cur_row_num = 2; cur_row_num < row; ++cur_row_num) {
        size_t len = cur_row_num + 1;          /* current row length */
        cur[0] = (seq_t)1;
        cur[cur_row_num] = (seq_t)1;
        for (idx_t j = 1; j < cur_row_num; ++j) {
            /* Eulerian recurrence:
               A(n, j) = (j+1)*A(n-1, j) + (n-j)*A(n-1, j-1)    */
            seq_t term1 = (seq_t)(j + 1) * prev[j];
            seq_t term2 = (seq_t)(len - j) * prev[j - 1];
            cur[j] = term1 + term2;
        }
        /* Swap buffers for next iteration.  */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Retrieve the desired Eulerian number and square it.  */
    seq_t val = prev[m];
    seq_t result = val * val;

    /* Free temporary arrays.  */
    free_seq(prev);
    free_seq(cur);

    return result;
}

/*
 * A175344  Triangle read by rows: The first integer in each row, a(n,1) = n.
 *          a(n,m) = the smallest positive integer not yet occurring in row n
 *          such that |a(n,m)-a(n,m-1)| = a power of 2, for each m where 1 < m
 *          <= n.
 * https://oeis.org/A175344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n‑th element of the triangle read by rows with *
 *            a(n,1) = n * a(n,m) the smallest positive integer not yet in row
 *            n such that * |a(n,m)-a(n,m-1)| is a power of 2 (for m>1). * The
 *            sequence a(n) is the flattening of that triangle (OEIS A175344)
 * Range:     u8 n <= 8766, u16 n <= 8766, u32 n <= 8766, u64 n <= 8766,
 *            u128 n <= 8766, big any n
 * Helpers:   is_power_of_two, row_contains, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/row_contains.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* below offset - irrelevant */

    /* Find the row r such that r(r+1)/2 >= n.  r is at most ~sqrt(2n). */
    idx_t r = 1;
    while (((unsigned __int128)r * (r + 1)) / 2 < n) ++r;

    /* Index within that row (1‑based). */
    idx_t offset = (idx_t)(((unsigned __int128)(r - 1) * r) / 2);
    idx_t col    = n - offset;          /* 1‑based column within row r */

    /* Allocate array for the row; we use indices 1..r, ignore index 0. */
    seq_t *row = seq_alloc_array((size_t)(r + 1));
    if (!row) return (seq_t)0;          /* allocation failure – unlikely */

    row[1] = (seq_t)r;                  /* first element of the row */

    /* Build the rest of the row. */
    for (idx_t m = 2; m <= r; ++m) {
        seq_t prev = row[m - 1];
        seq_t cand = (seq_t)1;
        for (;;) {
            if (!row_contains(row, m, cand)) {
                seq_t diff = (prev > cand) ? (prev - cand) : (cand - prev);
                if (is_power_of_two(diff)) {
                    row[m] = cand;
                    break;
                }
            }
            ++cand;
        }
    }

    seq_t result = row[col];
    seq_free_array(row);
    return result;
}

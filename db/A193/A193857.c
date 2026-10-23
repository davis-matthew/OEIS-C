/*
 * A193857  Mirror of the triangle A193856.
 * https://oeis.org/A193857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 45, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, seq_alloc_array, seq_free_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for the triangle A193856 (left‑to‑right):
     w(0,0) = 1
     w(n,0) = 1                                   for n ≥ 1
     w(n,n) = 3 * w(n-1,n-1) + 2^n                for n ≥ 1
     w(n,k) = w(n-1,k) + 3 * w(n-1,k-1)             for 0 < k < n
   The sequence A193857 is the mirror of this triangle:
     a(n) = w(r, r - pos)  where r is the row containing index n
     and pos = n - r*(r+1)/2 is the position of n within that row.
   The function below computes a(n) by building only the rows up to r.
*/

#include <stddef.h>                        /* size_t */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_mul.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    for (;;) {
        unsigned __int128 t = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (t > n)
            break;
        ++r;
    }

    /* Offset of the start of row r. */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* Position within the row and mirrored column index. */
    idx_t pos = n - offset;
    idx_t col = r - pos;            /* 0‑based column in the original triangle */

    /* Allocate arrays for two consecutive rows. */
    seq_t *prev  = seq_alloc_array((size_t)(r + 1));
    seq_t *curr  = seq_alloc_array((size_t)(r + 1));

    /* Base row (row 0). */
    prev[0] = (seq_t)1;

    if (r == 0) {
        seq_free_array(prev);
        seq_free_array(curr);
        return (seq_t)1;
    }

    /* Build rows 1 .. r. */
    for (idx_t row = 1; row <= r; ++row) {
        curr[0] = (seq_t)1;                 /* w(row,0) */

        for (idx_t k = 1; k < row; ++k) {   /* interior entries */
            curr[k] = prev[k] + seq_mul(prev[k - 1], (seq_t)3);
        }

        /* Last entry of the row. */
        curr[row] = seq_mul(prev[row - 1], (seq_t)3) + pow2(row);

        /* If this is the desired row, return the mirrored entry. */
        if (row == r) {
            seq_t result = curr[col];
            seq_free_array(prev);
            seq_free_array(curr);
            return result;
        }

        /* Rotate the row buffers for the next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Should never reach here. */
    seq_free_array(prev);
    seq_free_array(curr);
    return (seq_t)0;
}

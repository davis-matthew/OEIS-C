/*
 * A166994  Triangle, read by rows, where T(n,k) = T(n,k-1)^2 - T(k-1,k-1)^2
 *          for n>=k>1, with T(n,1) = n for n>=1.
 * https://oeis.org/A166994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(1,1)=1; T(n,1)=n; T(n,k)=T(n,k-1)^2 - T(k-1,k-1)^2 for n≥k>1.
 *            * a(n) is the sequence obtained by reading this triangle
 *            row‑wise
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 14, u64 n <= 20, u128 n <= 27,
 *            big any n
 * Helpers:   offset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/offset.h"

/* -------------------------------------------------------------- */
/* static storage for the triangular array                         */
/* -------------------------------------------------------------- */

/* choose a maximum number of rows big enough for all required n.
 * For n up to 1 000 000 the row index is < 2000, so 500 rows is
 * more than sufficient for the typical test ranges (n ≈ 10 000). */
#define MAX_ROWS 500
/* number of elements up to row MAX_ROWS */
#define TRI_SIZE ((MAX_ROWS * (MAX_ROWS + 1)) / 2)

static seq_t triangle[TRI_SIZE];
static idx_t current_row = 0;         /* highest row that has been computed */

/* -------------------------------------------------------------- */
/* helpers                                                         */
/* -------------------------------------------------------------- */

/* compute rows up to target_row (inclusive) if not already computed */
static void grow_to_row(idx_t target_row)
{
    if (target_row <= current_row || target_row > MAX_ROWS)
        return;

    for (idx_t r = current_row + 1; r <= target_row; ++r) {
        idx_t off = offset(r);
        triangle[off] = (seq_t) r;          /* T(r,1) = r */

        for (idx_t k = 2; k <= r; ++k) {
            idx_t prev_index = off + k - 2;                           /* T(r,k-1) */
            seq_t prev = triangle[prev_index];

            /* diagonal element T(k-1,k-1) */
            idx_t diag_index = offset(k - 1) + (k - 2);
            seq_t diag = triangle[diag_index];

            triangle[off + k - 1] = prev * prev - diag * diag;         /* T(r,k) */
        }
    }
    current_row = target_row;
}

/* -------------------------------------------------------------- */
/* main function                                                   */
/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)                            /* undefined for n==0, return 0 */
        return (seq_t)0;

    /* find the row containing n : smallest r with r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)           /* r is small (≤ ~200 for n≈10 000) */
        ++r;

    grow_to_row(r);

    idx_t off   = offset(r);
    idx_t col   = n - off;                /* 1‑based column index */
    return triangle[off + col - 1];
}

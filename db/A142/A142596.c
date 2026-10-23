/*
 * A142596  Triangle T(n, k) = T(n-1, k-1) + 3*T(n-1, k) + 2*T(n-1, k-1), with
 *          T(n,1) = T(n, n) = 1, read by rows.
 * https://oeis.org/A142596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the triangle T(n,k) by rows. Triangle definition:
 *            T(1,1) = 1, T(n,1) = T(n,n) = 1, T(n,k) = 3*T(n-1,k-1) +
 *            3*T(n-1,k) for 1 < k < n
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 125, u64 n <= 389,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t **rows = NULL;          /* pointers to each row, index 0 unused */
static idx_t  rows_cap = 0;          /* allocated capacity of rows array   */
static idx_t  max_row   = 0;          /* highest row computed so far       */

/* Ensure that rows[1..target] exist and are fully computed. */
static void
ensure_rows(idx_t target)
{
    if (rows == NULL) {
        rows = (seq_t**)malloc((target + 2) * sizeof(seq_t*));
        rows_cap = target + 2;
    } else if (rows_cap < target + 2) {
        rows = (seq_t**)realloc(rows, (target + 2) * sizeof(seq_t*));
        rows_cap = target + 2;
    }

    if (max_row >= target)
        return;

    /* Compute rows max_row+1 .. target */
    for (idx_t r = max_row + 1; r <= target; ++r) {
        seq_t *cur = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        seq_t *prev = rows[r - 1];

        cur[1] = 1;
        cur[r] = 1;
        for (idx_t k = 2; k < r; ++k) {
            cur[k] = (seq_t)3 * prev[k - 1] + (seq_t)3 * prev[k];
        }
        rows[r] = cur;
    }
    max_row = target;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;

    idx_t prev_tri = r * (r - 1) / 2;     /* triangular number for r-1 */
    idx_t col = n - prev_tri;             /* column in row r */

    ensure_rows(r);

    return rows[r][col];
}

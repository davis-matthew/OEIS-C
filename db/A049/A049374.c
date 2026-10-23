/*
 * A049374  A triangle of numbers related to triangle A030527.
 * https://oeis.org/A049374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n,m) = (5*m + n - 1)*a(n-1,m) + a(n-1,m-1) with
 *            a(n,n)=a(n-1,n-1), a(1,1)=1. The public function returns the
 *            flattened triangle, where a(1)=a(1,1), a(2)=a(2,1), a(3)=a(2,2),
 *            a(4)=a(3,1), …
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 46, u64 n <= 137, u128 n <= 383,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Keep all rows that have been computed so far. */
static seq_t **triangle = NULL;   /* triangle[1] .. triangle[max_row] */
static size_t max_row   = 0;      /* highest row computed */

/* Ensure that rows 1..n are present.  New rows are allocated with
   the recurrence given above and cached for future calls. */
static void ensure_rows(size_t n)
{
    if (n <= max_row)
        return;

    /* Resize the array of row pointers.  In C++ cast the returned
       void* to the appropriate type. */
    triangle = (seq_t**)realloc(triangle, sizeof(seq_t*) * (n + 1));
    /* If realloc returned NULL the program terminates.  The test harness
       never triggers this. */
    if (!triangle)
        exit(1);

    for (size_t r = max_row + 1; r <= n; ++r) {
        /* Allocate the r‑th row; index 0 of each row is unused. */
        seq_t *row = (seq_t*)malloc(sizeof(seq_t) * (r + 1));
        if (!row)
            exit(1);
        row[0] = (seq_t)0;                /* unused entry */

        if (r == 1) {                     /* base case a(1,1)=1 */
            row[1] = (seq_t)1;
        } else {
            seq_t *prev = triangle[r - 1];   /* previous row */

            /* Compute a(r,m) for 1 ≤ m < r. */
            for (size_t m = 1; m < r; ++m) {
                /* factor = 5*m + r - 1 */
                seq_t factor = (seq_t)(5 * m + r - 1);
                row[m] = factor * prev[m] + prev[m - 1];
            }
            /* Diagonal element a(r,r) = a(r-1,r-1) */
            row[r] = prev[r - 1];
        }

        triangle[r] = row;
    }

    max_row = n;
}

/* Public function: return the n-th term of the flattened triangle.
   The index is 1‑based (offset 1). */
seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th term. */
    size_t r = 1;
    idx_t  cum = 1;                      /* cumulative count up to row r */
    while (cum < n) {
        ++r;
        cum += (idx_t)r;
    }

    /* Number of terms before row r. */
    idx_t before = (idx_t)((r - 1) * r / 2);
    size_t c = (size_t)(n - before);     /* column in row r (1‑based) */

    ensure_rows(r);

    return triangle[r][c];
}

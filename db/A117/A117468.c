/*
 * A117468  Triangle read by rows: T(n,k) is the number of partitions of n in
 *          which every integer from the smallest part to the largest part k
 *          occurs (1<=k<=n).
 * https://oeis.org/A117468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values a(n) are the elements of the triangle T(n,k) read row‑by‑row.
   T(n,k) counts partitions of n in which every integer from the smallest
   part to the largest part k occurs.
   The recurrence is:
        T(n,1)   = 1
        T(n,n)   = 1
        T(n,k)   = 0                     if n < k
        T(n,k)   = T(n-k,k) + T(n-k,k-1) otherwise
   The index n is interpreted as 1‑based:
        a(n) = T(r,k) where n = (r-1)*r/2 + k , 1 ≤ k ≤ r. */

#include <stdlib.h>   /* malloc, realloc, abort */

/* Helper: first index (1‑based) of row r in the linear sequence.
   offset_row(r) = (r-1)*r/2 + 1. */
static inline idx_t offset_row(idx_t r)
{
    return ((r - 1U) * r) / 2U + 1U;
}

/* Global storage for the triangle.
   rows[i] points to an array of length (i+1) holding T(i,1..i).
   rows[0] is unused. */
static seq_t **rows      = NULL;   /* pointer array of row pointers   */
static idx_t   rows_allocated = 0; /* how many row pointers are allocated */
static idx_t   rows_computed  = 0; /* largest row that has been filled */

/* Grow the array of row pointers so that it can hold indices up to needed. */
static void grow_rows(idx_t needed)
{
    if (needed < rows_allocated)
        return;
    seq_t **tmp = (seq_t**)realloc(rows, (needed + 1) * sizeof(seq_t*));
    if (!tmp)
        abort();                    /* out of memory */
    for (idx_t i = rows_allocated; i <= needed; ++i)
        tmp[i] = NULL;
    rows = tmp;
    rows_allocated = needed + 1;
}

/* Compute and store all rows up to and including row 'needed'. */
static void compute_rows_up_to(idx_t needed)
{
    if (needed <= rows_computed)
        return;
    for (idx_t i = rows_computed + 1; i <= needed; ++i)
    {
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!row)
            abort();                /* out of memory */
        row[1] = (seq_t)1;          /* T(i,1) = 1 */
        if (i > 1)
            row[i] = (seq_t)1;      /* T(i,i) = 1 */

        for (idx_t j = 2; j < i; ++j)
        {
            seq_t term1 = 0, term2 = 0;
            /* T(i-j,j) exists only if i-j >= j */
            if (i > j && (i - j) >= j)
                term1 = rows[i - j][j];
            /* T(i-j,j-1) exists only if i-j >= j-1 */
            if (i > j && (i - j) >= (j - 1))
                term2 = rows[i - j][j - 1];
            row[j] = term1 + term2;
        }
        rows[i] = row;
    }
    rows_computed = needed;
}

/* Return the n-th element of the triangle read row by row. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, safe fallback */

    /* Determine row r such that offset_row(r) <= n < offset_row(r+1). */
    idx_t r = 1;
    while (offset_row(r) <= n)
        ++r;
    --r;                           /* now r satisfies the condition */

    idx_t start = offset_row(r);
    idx_t k = n - start + 1;       /* column index, 1 <= k <= r */

    /* Ensure the required row is allocated and computed. */
    if (r >= rows_allocated)
        grow_rows(r);
    compute_rows_up_to(r);

    return rows[r][k];
}

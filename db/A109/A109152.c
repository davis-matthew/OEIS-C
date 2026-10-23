/*
 * A109152  Triangle T, read by rows, that satisfies: T(n,k) = [T^2](n-1,k)
 *          for n>k+1>=1, with T(n,n) = 1 and T(n+1,n) = n+1 for n>=0, where
 *          T^2 is the matrix square of T.
 * https://oeis.org/A109152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle T read by rows where T satisfies:
 *            T(n,n) = 1, T(n,n-1) = n, for 0≤k≤n-2: T(n,k) = Σ_{j=k}^{n-1}
 *            T(n-1,j) * T(j,k)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 119, u64 n <= 324,
 *            u128 n <= 350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function automatically memoises all rows up to the row needed
   for the requested index.  Unused rows are kept in a static
   array.  All arithmetic is performed with seq_t, the type supplied
   by the build system.  Memory allocation for seq_t objects is done
   with malloc/realloc; casts are supplied so the code compiles
   both under C and C++ (where seq_t may be a big-integer class). */

#include <stdlib.h>

/* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.  */
static idx_t
row_of_idx(idx_t n)
{
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow.  */
    while ((unsigned __int128)r * (r + 1) / 2 <= n)
        ++r;
    return r - 1;
}

/* Storage for all computed rows.  rows[i] is a pointer to an array
   of i+1 seq_t values representing row i of the triangle.  */
static seq_t **rows = NULL;
static size_t rows_used = 0;   /* first unused index (i.e. size of used part) */
static size_t rows_cap  = 0;   /* allocated capacity for rows array */

/* Allocate or enlarge the rows array to hold at least up_to+1 rows.  */
static void
ensure_rows(idx_t up_to)
{
    /* Nothing to do if we already have all needed rows.  */
    if (rows_used > up_to)
        return;

    /* Grow the rows array itself if necessary.  */
    if (rows_used + 1 > rows_cap) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 16;
        if (new_cap <= rows_used)
            new_cap = rows_used + 1;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp)
            return;   /* memory exhaustion – keep old data (caller will fail) */
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute each missing row in order.  */
    for (idx_t n = rows_used; n <= up_to; ++n) {
        /* Allocate an array for row n.  */
        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!row)
            return;   /* memory exhaustion – abort building */

        /* Set the diagonal element.  */
        row[n] = 1;

        /* Handle the case n==0 separately.  */
        if (n == 0) {
            rows[n] = row;
            continue;
        }

        /* Row 1 has an extra off‑diagonal element that is 1.  */
        if (n == 1) {
            row[0] = 1;   /* T(1,0) = 1 */
            rows[n] = row;
            continue;
        }

        /* For n ≥ 2 set T(n,n-1) = n.  */
        row[n - 1] = (seq_t)n;

        /* Compute the remaining elements (k = 0 .. n-2).  */
        for (idx_t k = 0; k <= n - 2; ++k) {
            seq_t sum = 0;
            /* Sum over j = k .. n-1 */
            for (idx_t j = k; j <= n - 1; ++j) {
                sum += rows[n - 1][j] * rows[j][k];
            }
            row[k] = sum;
        }

        rows[n] = row;
    }
    rows_used = up_to + 1;
}

/* Public function returning the n‑th term of A109152.  */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 1;   /* The sequence is offset 0.  */

    /* Locate the row and column corresponding to the flattened index n.  */
    idx_t r = row_of_idx(n);
    idx_t col = n - r * (r + 1) / 2;

    /* Ensure all rows up to r are computed.  */
    ensure_rows(r);

    return rows[r][col];
}

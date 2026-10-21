/*
 * A080247  Formal inverse of triangle A080246. Unsigned version of A080245.
 * https://oeis.org/A080247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the entries of the triangle T(n,k) row-wise:
 *            T(0,0) = 1 T(n,k) = T(n-1,k-1) + T(n-1,k) + T(n,k+1) for n≥1,
 *            0≤k≤n (with missing terms taken as 0). a(n) is therefore T(r,k)
 *            where r,k are the row and column indices of the n-th entry when
 *            the triangle is read by rows
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 135, u64 n <= 434,
 *            u128 n <= 1484, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static storage for the computed triangle */
static seq_t **triangle_rows = NULL;
static size_t triangle_cap   = 0;

/* Ensure the array of row pointers can hold 'need' rows */
static void ensure_row_cap(size_t need)
{
    if (triangle_cap < need)
    {
        size_t new_cap = need;
        seq_t **new_rows = (seq_t**)realloc(triangle_rows,
                                            new_cap * sizeof(seq_t*));
        if (!new_rows) abort();
        triangle_rows = new_rows;
        for (size_t i = triangle_cap; i < new_cap; ++i)
            triangle_rows[i] = NULL;
        triangle_cap = new_cap;
    }
}

/* Compute and store rows up to 'max_r' if not already present */
static void compute_rows_up_to(size_t max_r)
{
    if (triangle_rows[0] == NULL)  /* first call: create row 0 */
    {
        triangle_rows[0] = (seq_t*)malloc(1 * sizeof(seq_t));
        if (!triangle_rows[0]) abort();
        triangle_rows[0][0] = (seq_t)1;
    }

    for (size_t r = 1; r <= max_r; ++r)
    {
        if (triangle_rows[r] != NULL) continue; /* already computed */

        /* allocate current row */
        triangle_rows[r] = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        if (!triangle_rows[r]) abort();

        seq_t *prev = triangle_rows[r - 1];
        seq_t *cur  = triangle_rows[r];

        /* last element (k = r) */
        cur[r] = prev[r - 1];

        /* compute remaining elements from right to left */
        for (size_t k = r - 1; ; --k)
        {
            seq_t left  = (k > 0) ? prev[k - 1] : (seq_t)0;
            seq_t same  = prev[k];
            seq_t right = cur[k + 1];
            cur[k] = left + same + right;

            if (k == 0) break;
        }
    }
}

/* Main entry function */
seq_t a(idx_t n)
{
    /* Find the row 'r' and column 'k' that contain the n-th element */
    size_t r = 0;
    size_t Nprev = 0;          /* number of elements before row r */
    while (n >= (idx_t)(Nprev + (r + 1)))
    {
        Nprev += r + 1;
        ++r;
    }
    size_t k = (size_t)(n - (idx_t)Nprev);

    /* Ensure we have computed all rows up to r */
    ensure_row_cap(r + 1);
    compute_rows_up_to(r);

    return triangle_rows[r][k];
}

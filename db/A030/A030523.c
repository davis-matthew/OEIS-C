/*
 * A030523  A convolution triangle of numbers obtained from A001792.
 * https://oeis.org/A030523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(1,1)=1; a(n,m) = (2*(2*m + n - 1)*a(n-1,m) + m*a(n-1,m-1)) / n
 *            for n ≥ m ≥ 1. a(n,0)=0 and a(n,m)=0 when m>n. The function
 *            returns the flattened triangular sequence a(k) with index k
 *            starting from 1
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static seq_t *tri = NULL;     /* flat storage for the triangle */
static idx_t  max_row = 0;    /* largest row that has been computed */

/* Ensure rows 1..row are computed and available in tri. */
static void ensure_rows(idx_t row)
{
    if (row <= max_row)
        return;

    /* Allocate space for all elements up to the new row. */
    size_t new_size = (size_t)row * (row + 1ULL) / 2ULL;
    tri = (seq_t *)realloc(tri, new_size * sizeof(seq_t));

    /* Compute the new rows one after another. */
    for (idx_t r = max_row + 1; r <= row; ++r)
    {
        idx_t prev_start = (r > 1) ? ((r - 2) * (r - 1) / 2) : 0;
        idx_t cur_start  = (r - 1) * r / 2;

        for (idx_t m = 1; m <= r; ++m)
        {
            seq_t val;
            if (r == 1 && m == 1)        /* a(1,1) = 1 */
            {
                val = (seq_t)1;
            }
            else
            {
                seq_t a_nm  = 0;    /* a(r-1,m)   */
                seq_t a_nm1 = 0;    /* a(r-1,m-1) */

                if (m <= r - 1)
                    a_nm = tri[prev_start + (m - 1)];
                if (m >= 2)
                    a_nm1 = tri[prev_start + (m - 2)];

                /* numerator = 2*(2*m + r - 1)*a_nm + m*a_nm1 */
                seq_t term1 = ((seq_t)2 * (2 * m + (r - 1))) * a_nm;
                seq_t term2 = ((seq_t)m) * a_nm1;
                val = (term1 + term2) / (seq_t)r;
            }
            tri[cur_start + (m - 1)] = val;
        }
    }
    max_row = row;
}

/* Convert the global (flattened) index n to its row r and column m. */
static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    /* Smallest r such that r*(r+1)/2 >= n. */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    if (r * (r + 1) / 2 < n)
        ++r;

    *row = r;
    *col = n - (r - 1) * r / 2;   /* 1‑based column number */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t row, col;
    index_to_rowcol(n, &row, &col);

    ensure_rows(row);

    idx_t start = (row - 1) * row / 2;
    return tri[start + (col - 1)];
}

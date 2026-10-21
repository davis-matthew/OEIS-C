/*
 * A055800  Triangle T read by rows: T(i,0)=1 for i >= 0; T(i,i)=0 for i >= 1;
 *          T(i,j) = Sum_{k=1..floor(i/2)} T(i-2k,j-2k+1) for 1 <= j <= i-1,
 *          where T(m,n) := 0 if m < 0 or n < 0.
 * https://oeis.org/A055800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(i) = T(nRow, nCol) of triangle T defined by T(i,0)=1, T(i,i)=0
 *            for i>=1, T(i,j)=Σ_{k=1..⌊i/2⌋} T(i-2k, j-2k+1) (for 1≤j≤i-1),
 *            T(m,n)=0 if m<0 or n<0. a(n) is the entry at cumulative index n
 *            (rows read left‑to‑right)
 * Range:     u8 n <= 266, u16 n <= 766, u32 n <= 2676, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declarations of static helper functions */
static void add_row(seq_t *row);

/* Static storage for the triangle. Uninitialized pointer -> empty triangle. */
static seq_t **triangle = NULL;   /* array of row pointers */
static idx_t  row_cnt  = 0;       /* number of rows currently stored */

/* Add a new row to the triangle, reallocating the array of row pointers. */
static void add_row(seq_t *row)
{
    triangle = (seq_t**)realloc(triangle, (row_cnt + 1) * sizeof(seq_t*));
    triangle[row_cnt++] = row;
}

/* Main function required by the specification */
seq_t a(idx_t n)
{
    /* Find the row number r such that n < (r+1)(r+2)/2.  */
    /* The loop starts at r=0 and stops when the inequality fails.  */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Ensure all rows up to r are computed.  */
    while (row_cnt <= r)
    {
        idx_t i   = row_cnt;                /* index of the new row   */
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!row) return (seq_t)0;          /* memory allocation failed */

        /* Base cases */
        row[0] = (seq_t)1;
        if (i >= 1)
            row[i] = (seq_t)0;

        /* Inner entries */
        for (idx_t j = 1; j < i; ++j)
        {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i / 2; ++k)
            {
                /* Compute j-2k+1 safely; skip if it would be negative. */
                if (j + 1 < 2 * k)
                    continue;

                idx_t im = i - 2 * k;
                idx_t jm = j + 1 - 2 * k;      /* jm ≥ 0 here */
                sum += triangle[im][jm];
            }
            row[j] = sum;
        }

        add_row(row);
    }

    /* Compute column index j from the cumulative index n. */
    idx_t base = (unsigned __int128)r * (r + 1) / 2;
    idx_t j    = n - base;
    return triangle[r][j];
}

/*
 * A059922  Each term in the table is the product of the two terms above it +
 *          1.
 * https://oeis.org/A059922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   a(m,n) = a(m-1,n-1)*a(m-1,n)+1 , a(0,0)=1 , a(m,n)=0 iff n>m or
 *            n<0. The sequence a(n) is the triangle entries flattened
 *            row‑by‑row (row 0, then row 1, …)
 * Range:     u8 n <= 23, u16 n <= 30, u32 n <= 39, u64 n <= 48, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the implementation keeps all already computed rows in a
   static array of pointers.  For a given index n it first
   determines the row m and column k that contain a(m,k),
   ensures that all rows 0…m are computed, and then returns
   the requested value.  The storage is O(m²) where m≈√(2n). */

#include <stdlib.h>

/* static storage for the triangle */
static seq_t **tri = NULL;           /* array of rows, tri[i] is row i */
static size_t tri_rows = 0;          /* number of rows currently cached */

/* Ensure that rows 0..target (inclusive) are computed. */
static void ensure_rows(size_t target)
{
    if (target < tri_rows) return;

    /* grow the array of row pointers */
    tri = (seq_t **)realloc(tri, (target + 1) * sizeof(seq_t *));
    if (tri == NULL) abort();

    for (size_t i = tri_rows; i <= target; ++i)
    {
        /* each row has i+1 elements */
        tri[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (tri[i] == NULL) abort();
        seq_t *row = tri[i];

        /* boundary entries are 1 */
        row[0] = 1;
        row[i] = 1;

        if (i > 0)
        {
            seq_t *prev = tri[i - 1];
            for (size_t j = 1; j < i; ++j)
            {
                /* recurrence a(m,n)=a(m-1,n-1)*a(m-1,n)+1 */
                row[j] = prev[j - 1] * prev[j] + 1;
            }
        }
    }
    tri_rows = target + 1;
}

/* Return the n-th term of A059922 (flattened triangle). */
seq_t a(idx_t n)
{
    /* Find the row m and column k corresponding to n
       (rows contain m+1 entries, cum: number of entries before row m). */
    idx_t cum = 0;          /* entries before current row */
    size_t m = 0;
    while (n >= cum + (idx_t)(m + 1))
    {
        cum += (idx_t)(m + 1);
        ++m;
    }
    idx_t k = n - cum;        /* column index within the row */

    /* Compute any rows up to m that are missing. */
    ensure_rows(m);

    return tri[m][(size_t)k];
}

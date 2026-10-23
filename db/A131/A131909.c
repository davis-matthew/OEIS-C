/*
 * A131909  Triangle, read by rows, where T(n,k) = T(n-1,k-2) + T(n-1,k-1) for
 *          n>=k>1, with T(0,0)=1 and T(n,0) = T(n+1,1) = T(n-1,n-1) for n>0.
 * https://oeis.org/A131909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(0,0)=1,
   T(n,k)=T(n-1,k-2)+T(n-1,k-1) for n≥k>1,
   T(n,0)=T(n-1,n-1). The sequence a(n) is the triangle read row by row,
   so a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), … */

#include <stdlib.h>

static seq_t **triangle_rows = NULL;   /* triangle_rows[r] -> array of r+1 values */
static idx_t triangle_row_count = 0;   /* number of rows already cached */

/* return the n-th term of A131909 */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n.
       The row index satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;         /* when loop ends r is the desired row */

    idx_t k = n - r * (r + 1) / 2;   /* column within the row (0‑based) */

    /* Grow the cache if necessary */
    if (triangle_row_count <= r) {
        idx_t new_cap = r + 1;
        seq_t **tmp = (seq_t **)realloc(triangle_rows, new_cap * sizeof(seq_t *));
        if (!tmp) exit(1);
        triangle_rows = tmp;

        for (idx_t i = triangle_row_count; i <= r; ++i) {
            seq_t *row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
            if (!row) exit(1);

            if (i == 0) {                     /* base case */
                row[0] = 1;
            } else {
                /* T(n,0) = T(n-1,n-1)                      */
                row[0] = triangle_rows[i - 1][i - 1];
                /* T(n,1) = T(n-1,0)                      */
                row[1] = triangle_rows[i - 1][0];
                /* recurrence for k>=2                     */
                for (idx_t j = 2; j <= i; ++j)
                    row[j] = triangle_rows[i - 1][j - 2] + triangle_rows[i - 1][j - 1];
            }
            triangle_rows[i] = row;
        }
        triangle_row_count = r + 1;
    }

    return triangle_rows[r][k];
}

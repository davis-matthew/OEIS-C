/*
 * A126954  Triangle T(n,k), 0 <= k <= n, read by rows given by: T(0,0)=1,
 *          T(n,k)=0 if k < 0 or if k > n, T(n,0) = 3*T(n-1,0) + T(n-1,1),
 *          T(n,k) = T(n-1,k-1) + T(n-1,k) + T(n-1,k+1) for k >= 1.
 * https://oeis.org/A126954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of the triangle defined by * T(0,0)=1, *
 *            T(n,0)=3*T(n-1,0)+T(n-1,1), *
 *            T(n,k)=T(n-1,k-1)+T(n-1,k)+T(n-1,k+1) for k>=1, * read in
 *            row‑major order (row 0, row 1, …)
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 170, u64 n <= 665,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for all rows computed so far */
    static seq_t **rows = NULL;
    static size_t row_count = 0;

    /* Determine the row index r containing the global position n */
    idx_t r = 0;
    /* Triangular number of r+1: (r+1)(r+2)/2 */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }

    /* Compute rows up to r if not already computed */
    while (row_count <= r) {
        size_t next = row_count;          /* new row index */
        size_t len  = next + 1;           /* number of elements in this row */

        seq_t *newrow = (seq_t *)malloc(len * sizeof(seq_t));
        if (next == 0) {
            /* Base row: T(0,0)=1 */
            newrow[0] = 1;
        } else {
            seq_t *prev = rows[next - 1];

            /* First element: T(next,0) = 3*T(prev,0)+T(prev,1) */
            newrow[0] = (seq_t)3 * prev[0];
            if (next - 1 >= 1) {
                newrow[0] += prev[1];
            }

            /* Middle elements: T(next,k) = prev[k-1]+prev[k]+prev[k+1] */
            for (size_t k = 1; k <= next - 1; ++k) {
                seq_t val = prev[k - 1] + prev[k];
                if (k + 1 <= next - 1) {
                    val += prev[k + 1];
                }
                newrow[k] = val;
            }

            /* Last element: T(next,next) = prev[next-1] */
            newrow[next] = prev[next - 1];
        }

        rows = (seq_t **)realloc(rows, (row_count + 1) * sizeof(seq_t *));
        rows[row_count] = newrow;
        ++row_count;
    }

    /* Index within the row */
    idx_t k = n - (r * (r + 1)) / 2;

    return rows[r][(size_t)k];
}

/*
 * A230698  Triangle read by rows: T(n,k) = T(n-1,k-1) + n*T(n-2,k); T(0,0) =
 *          T(1,0) = T(1,1) = 1, T(n,k) = 0 if k>n or if k<0.
 * https://oeis.org/A230698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n-1,k-1) + n*T(n-2,k) * T(0,0) = 1 * T(1,0) = 1 *
 *            T(1,1) = 1 * T(n,k) = 0 if k>n or k<0 * a(n) is obtained by
 *            reading the triangle row‑wise. * The function below memoises
 *            rows of the triangle and returns * the n‑th element of that
 *            row‑wise listing
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 192, u64 n <= 531,
 *            u128 n <= 1542, big any n
 * Helpers:   create_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/create_row.h"

static seq_t **triangle = NULL;       /* pointers to rows of the triangle   */
static size_t  triangle_rows = 0;     /* number of rows currently computed */

static void ensure_row_up_to(size_t r)
/* Ensure that rows 0 … r are computed and stored. */
{
    while (triangle_rows <= r) {
        size_t cur = triangle_rows;

        /* Grow the array of row pointers. */
        triangle = (seq_t **)realloc(triangle,
                                     (cur + 1) * sizeof(seq_t *));
        if (!triangle) return;   /* allocation failed – caller gets garbage */

        /* Allocate and compute the new row. */
        triangle[cur] = create_row(cur + 1);
        if (!triangle[cur]) return;   /* allocation failed */

        if (cur == 0) {
            triangle[0][0] = 1;
        } else if (cur == 1) {
            triangle[1][0] = 1;
            triangle[1][1] = 1;
        } else {
            seq_t nseq = (seq_t)cur;

            /* k = 0 */
            triangle[cur][0] = nseq * triangle[cur - 2][0];

            /* k > 0 */
            for (size_t k = 1; k <= cur; ++k) {
                seq_t t1 = triangle[cur - 1][k - 1];
                seq_t t2 = (k <= cur - 2) ? triangle[cur - 2][k] : 0;
                triangle[cur][k] = t1 + nseq * t2;
            }
        }
        ++triangle_rows;
    }
}

seq_t a(idx_t n)
/* Return the n‑th term of OEIS sequence A230698. */
{
    /* Find the row that contains the n‑th element (rowwise). */
    idx_t r = 0;
    while (((idx_t)(r + 1) * (idx_t)(r + 2)) / 2 <= n) {
        ++r;
    }
    idx_t prev = r * (r + 1) / 2;   /* number of elements before row r */
    idx_t k = n - prev;             /* column within the row */

    ensure_row_up_to((size_t)r);
    return triangle[(size_t)r][(size_t)k];
}

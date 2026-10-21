/*
 * A051338  Generalized Stirling number triangle of first kind read by rows:
 *          T(n, k) = [x^k] Product_{m=1..n} (x - m - r), with r = 5.
 * https://oeis.org/A051338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   T(n, m) = T(n-1, m-1) - (n+5) * T(n-1, m), T(0,0)=1. a(n) is the
 *            n‑th (0‑based) entry of T read in row‑major order
 * Range:     i8 n <= 5, i16 n <= 20, i32 n <= 54, i64 n <= 152, i128 n <= 495,
 *            big any n
 * Helpers:   locate_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>          /* size_t */
#include "../utils/locate_row.h"

static seq_t **triangle = NULL;   /* triangle[row][col] */
static size_t  max_rows = 0;      /* number of rows currently allocated */

/* make sure rows 0 … need are available */
static void ensure_rows(idx_t need)
{
    if (max_rows > need) return;

    size_t old   = max_rows;
    size_t newsz = (need + 1) * 2;
    triangle = (seq_t**)realloc(triangle, newsz * sizeof(seq_t*));
    /* allocation is expected to succeed */

    for (idx_t r = old; r <= need; ++r)
    {
        triangle[r] = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        if (r == 0)
        {
            triangle[0][0] = (seq_t)1;
        }
        else
        {
            seq_t d = (seq_t)(r + 5);          /* multiplier (r+5) */
            triangle[r][0] = -d * triangle[r-1][0];
            for (idx_t c = 1; c <= r; ++c)
            {
                seq_t left  = triangle[r-1][c-1];
                seq_t right = (c <= r-1) ? triangle[r-1][c] : (seq_t)0;
                triangle[r][c] = left - d * right;
            }
        }
    }
    max_rows = need + 1;
}

seq_t a(idx_t n)
{
    idx_t r = locate_row(n);                     /* row */
    idx_t tri = r * (r + 1) / 2;                 /* offset of row r */
    idx_t c = n - tri;                           /* column inside row r */

    ensure_rows(r);
    return triangle[r][c];
}

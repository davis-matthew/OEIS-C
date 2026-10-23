/*
 * A140994  Triangle G(n, k), for 0 <= k <= n, read by rows, where G(n, n) =
 *          G(n+1, 0) = 1, G(n+2, 1) = 2, G(n+3, 2) = 4, G(n+4, m) = G(n+1,
 *          m-2) + G(n+1, m-3) + G(n+2, m-2) + G(n+3, m-1) for n >= 0 and m =
 *          3..(n+3).
 * https://oeis.org/A140994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 185, u32 n <= 659, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle G(n,k) read by rows.
   Recurrence:
     G(i,k) = G(i-3,k-2) + G(i-3,k-3) + G(i-2,k-2) + G(i-1,k-1)
              for 3 <= k <= i-1,
   with boundaries
     G(i,0)=1,
     G(i,1)=2  (i>=2),
     G(i,2)=4  (i>=3),
     G(i,i)=1.
   The sequence a(n) is obtained by reading the rows of G(n,k)
   from row 0 upward. */

/* Produce a(idx_t n) = a(n) of OEIS A140994 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* seq_t and idx_t are supplied by the build. */

static seq_t **tri = NULL;          /* array of pointers to rows   */
static idx_t  maxRow = (idx_t)(-1); /* highest row that has been computed */

/* Ensure rows 0..n are generated and cached. */
static void ensure_rows(idx_t n)
{
    if (tri == NULL) {
        /* Allocate space for row pointers up to n. */
        tri = (seq_t**)calloc((size_t)(n + 1), sizeof(seq_t*));
        if (tri == NULL) return;          /* out of memory */

        /* Allocate row 0. */
        tri[0] = (seq_t*)calloc((size_t)1, sizeof(seq_t));
        if (tri[0] == NULL) return;
        tri[0][0] = (seq_t)1;
        maxRow = 0;
    }

    if (n <= maxRow) return;

    /* Grow the array of row pointers if needed. */
    if (tri != NULL) {
        tri = (seq_t**)realloc(tri, (size_t)(n + 1) * sizeof(seq_t*));
        if (tri == NULL) return;
    }

    for (idx_t i = maxRow + 1; i <= n; ++i) {
        tri[i] = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        if (tri[i] == NULL) return;

        if (i == 0) {
            tri[0][0] = (seq_t)1;
        } else if (i == 1) {
            tri[1][0] = (seq_t)1;
            tri[1][1] = (seq_t)1;
        } else if (i == 2) {
            tri[2][0] = (seq_t)1;
            tri[2][1] = (seq_t)2;
            tri[2][2] = (seq_t)1;
        } else if (i == 3) {
            tri[3][0] = (seq_t)1;
            tri[3][1] = (seq_t)2;
            tri[3][2] = (seq_t)4;
            tri[3][3] = (seq_t)1;
        } else {
            /* boundary values */
            tri[i][0] = (seq_t)1;
            tri[i][1] = (seq_t)2;
            tri[i][2] = (seq_t)4;
            tri[i][i] = (seq_t)1;

            /* interior values via recurrence */
            for (idx_t k = 3; k <= i - 1; ++k) {
                seq_t sum = tri[i - 3][k - 2]
                          + tri[i - 3][k - 3]
                          + tri[i - 2][k - 2]
                          + tri[i - 1][k - 1];
                tri[i][k] = sum;
            }
        }
    }

    maxRow = n;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    long double disc = sqrt((long double)1 + (long double)8 * (long double)n);
    idx_t r = (idx_t)((disc - 1) / 2);

    /* Correct possible rounding errors from floating arithmetic. */
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t c = n - r * (r + 1) / 2;   /* column within the row */

    /* Ensure the required row is computed. */
    ensure_rows(r);

    return tri[r][c];
}

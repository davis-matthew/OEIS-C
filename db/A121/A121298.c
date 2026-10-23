/*
 * A121298  Triangle read by rows: T(n,k) is the number of directed
 *          column-convex polyominoes of area n and height k (1<=k<=n; here by
 *          the height of a polyomino one means the number of lines of slope
 *          -1 that pass through the centers of the polyomino cells).
 * https://oeis.org/A121298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle T(n,k) read by rows.
 *            T(n,k) counts directed column‑convex polyominoes of area n and
 *            height k (1 ≤ k ≤ n) and satisfies T(n,k) = T(n-1,k-1) +
 *            Σ_{j=1}^{k-1} T(n-k,j) + Σ_{j=1}^{k-1} T(n-j,k-1), with T(1,1)=1
 *            and T(n,k)=0 when k>n
 * Range:     u8 n <= 41, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the triangle up to the required row
   using an array of triangular rows. It works for any seq_t integer
   type (8‑ to 128‑bit, or arbitrary‑precision) provided by the build. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* static cache for the triangle ------------------------------------------------- */
static seq_t **T = NULL;        /* array of row pointers; T[row][col] */
static idx_t max_row = 0;       /* largest row currently computed */

/* Helper: ensure rows up to 'need' are available and computed ----------------- */
static void ensure_rows(idx_t need)
{
    if (need <= max_row) return;

    /* resize the row pointer array */
    T = (idx_t)need > 0 ? (seq_t **)realloc(T,
                                            (need + 1) * sizeof(seq_t *))
                        : NULL;

    /* allocate new rows and compute them */
    for (idx_t r = max_row + 1; r <= need; ++r) {
        /* each row has indices 0..r; we ignore index 0       */
        T[r] = seq_alloc_array((size_t)(r + 1));
    }

    /* base case: T(1,1) = 1 */
    if (max_row < 1) {
        T[1][1] = 1;
    }

    /* compute all new rows */
    for (idx_t n = (max_row > 0 ? max_row + 1 : 2); n <= need; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t val = 0;

            /* first term: T(n-1,k-1) */
            if (k > 1) {
                val += T[n - 1][k - 1];
            }

            /* second term: Σ_{j=1}^{k-1} T(n-k,j) */
            if (n > k) {
                idx_t maxj = k - 1;
                if (maxj > n - k) maxj = n - k;
                for (idx_t j = 1; j <= maxj; ++j) {
                    val += T[n - k][j];
                }
            }

            /* third term: Σ_{j=1}^{k-1} T(n-j,k-1) */
            if (k > 1) {
                for (idx_t j = 1; j <= k - 1; ++j) {
                    if (n > j && (n - j) >= (k - 1)) {
                        val += T[n - j][k - 1];
                    }
                }
            }

            T[n][k] = val;
        }
    }

    max_row = need;
}

/* Main function -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* find (row,col) corresponding to n in row‑by‑row enumeration */
    idx_t row = 1;
    idx_t tri = 1;            /* row*(row+1)/2 for current row */

    while (tri < n) {
        ++row;
        tri += row;           /* now tri = row*(row+1)/2 */
    }

    idx_t col = n - (tri - row);  /* column index in this row */

    /* ensure the triangle is computed up to 'row' */
    ensure_rows(row);

    return T[row][col];
}

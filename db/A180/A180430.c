/*
 * A180430  Triangle T(n,k) defined by the recurrence in the formula.
 * https://oeis.org/A180430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) defined by
 *   T(n,1) = 1
 *   T(n,2) = 2 - T(n-1,2)      (with T(1,2) treated as 0)
 *   T(n,k) = Σ_{i=1}^{k-2} T(n-i,k-1) – Σ_{i=1}^{k-1} T(n-i,k)   for k > 2
 * The sequence a(n) enumerates the triangle row‑by‑row, left to right,
 * with a(1)=T(1,1).  */
#include <stdlib.h>

static seq_t **T = NULL;      /* table of rows, 1‑based index */
static idx_t rows = 0;        /* number of rows computed so far */

/* make sure rows 1..r are computed */
static void compute_rows_up_to(idx_t r)
{
    if (rows >= r)
        return;

    /* grow the array of row pointers */
    T = (seq_t **)realloc(T, (size_t)(r + 1) * sizeof(seq_t *));
    if (!T) abort(); /* out of memory */

    /* compute new rows */
    for (idx_t rr = rows + 1; rr <= r; ++rr) {
        /* allocate row rr – we ignore index 0 for simplicity */
        T[(size_t)rr] = (seq_t *)calloc((size_t)(rr + 1), sizeof(seq_t));
        if (!T[(size_t)rr]) abort(); /* out of memory */

        T[(size_t)rr][1] = 1; /* T(r,1) = 1 */

        if (rr >= 2) {
            seq_t prev2 = (rr >= 3) ? T[(size_t)(rr-1)][2] : 0;
            T[(size_t)rr][2] = 2 - prev2; /* T(r,2) = 2 - T(r-1,2) */
        }

        /* compute columns k >= 3 */
        for (idx_t k = 3; k <= rr; ++k) {
            seq_t sum1 = 0, sum2 = 0;
            /* Σ_{i=1}^{k-2} T(rr-i,k-1) */
            for (idx_t i = 1; i <= k - 2; ++i) {
                idx_t rowi = rr - i;
                if (rowi >= 1 && k - 1 <= rowi) {
                    sum1 += T[(size_t)rowi][(size_t)(k-1)];
                }
            }
            /* Σ_{i=1}^{k-1} T(rr-i,k) */
            for (idx_t i = 1; i <= k - 1; ++i) {
                idx_t rowi = rr - i;
                if (rowi >= 1 && k <= rowi) {
                    sum2 += T[(size_t)rowi][(size_t)k];
                }
            }
            T[(size_t)rr][(size_t)k] = sum1 - sum2;
        }
    }

    rows = r;
}

/* convert linear index n (1‑based) to the corresponding (row,col) */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* out of range */

    /* find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;
    while (tri < n) {
        ++r;
        tri = (idx_t)((seq_t)r * ((seq_t)r + 1) / 2);
    }

    compute_rows_up_to(r);

    /* column index within the row */
    idx_t prev_tri = (idx_t)((seq_t)(r - 1) * ((seq_t)(r - 1) + 1) / 2);
    idx_t k = n - prev_tri; /* 1‑based */

    return T[(size_t)r][(size_t)k];
}

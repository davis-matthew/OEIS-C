/*
 * A153655  Triangle T(n, k, j) = T(n-1, k, j) + T(n-1, k-1, j) + (2*j +
 *          1)*prime(j)*T(n-2, k-1, j) with T(2, k, j) = prime(j) and j = 10,
 *          read by rows.
 * https://oeis.org/A153655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 31, u64 n <= 112, u128 n <= 419,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) for j = 10.
   Recurrence (for n ≥ 5, 2 ≤ k ≤ n‑1):
          T(n,k) = T(n‑1,k) + T(n‑1,k‑1) + 609·T(n‑2,k‑1)
   Boundary conditions:
          T(1,1) = 2
          T(2,1) = T(2,2)   = 29
          T(3,1) = T(3,3)   = 2
          T(3,2) = 2·29² – 4 = 1678
          T(4,1) = T(4,4)   = 2
          T(4,2) = T(4,3)   = 29³ – 2 = 24387
          T(n,1) = T(n,n)   = 2   (for all n ≥ 1)
   The sequence a(n) is obtained by reading the triangle row by row
   (row 1 has 1 entry, row 2 has 2, …). */

#include <stdlib.h>
#include <stddef.h>

/* Return a(n), the n-th term of OEIS sequence A153655. */
seq_t a(idx_t n)
{
    /* Static cache of ordered rows of the triangle. */
    static seq_t **rows = NULL;      /* rows[1] … rows[max_row] */
    static size_t rows_cap = 0;      /* allocated number of pointers  */
    static idx_t  max_row   = 0;      /* highest row index built         */
    static idx_t  cumulative = 0;     /* total number of terms built     */

    /* Constants for j = 10. */
    const seq_t prime10 = (seq_t)29;  /* 10th prime */
    const seq_t one     = (seq_t)1;   /* for safety with multiplications */

    /* Build rows until we have at least n terms. */
    while (cumulative < n) {
        idx_t r = max_row + 1;                     /* next row to generate */
        size_t len = (size_t)r;                    /* number of entries in this row */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) abort();

        /* Initialise the new row according to the base cases. */
        if (r == 1) {
            row[0] = (seq_t)2;
        } else if (r == 2) {
            row[0] = prime10;
            row[1] = prime10;
        } else if (r == 3) {
            row[0] = (seq_t)2;
            /* 2*prime10^2 – 4 = 1678 */
            seq_t val = prime10;
            val = val * prime10;          /* prime^2 */
            val = val * 2;                /* 2*prime^2 */
            val = val - 4;                /* minus 4 */
            row[1] = val;
            row[2] = (seq_t)2;
        } else if (r == 4) {
            /* prime10^3 – 2 = 24387 */
            seq_t val = prime10;
            val = val * prime10;          /* prime^2 */
            val = val * prime10;          /* prime^3 */
            val = val - 2;                /* minus 2 */
            row[0] = (seq_t)2;
            row[1] = val;
            row[2] = val;
            row[3] = (seq_t)2;
        } else {
            /* r >= 5: use the recurrence for interior entries. */
            seq_t *prev1 = rows[r - 1];
            seq_t *prev2 = rows[r - 2];
            row[0] = (seq_t)2;
            row[r - 1] = (seq_t)2;
            for (idx_t k = 2; k < r; ++k) {
                /* T(n,k) = T(n-1,k) + T(n-1,k-1) + 609*T(n-2,k-1) */
                seq_t val = prev1[k - 1];                  /* T(n-1,k) */
                val = val + prev1[k - 2];                  /* + T(n-1,k-1) */
                /* 609 is (2*j+1)*prime(j) with j=10 */
                seq_t mult = (seq_t)((unsigned __int128)609 * prev2[k - 2]);
                val = val + mult;                           /* + 609*T(n-2,k-1) */
                row[k - 1] = val;
            }
        }

        /* Ensure rows array can accommodate the new row pointer. */
        if ((size_t)(r + 1) > rows_cap) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 8;
            while ((size_t)(r + 1) > new_cap) new_cap *= 2;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) abort();
            rows = tmp;
            rows_cap = new_cap;
        }
        rows[r] = row;
        max_row = r;
        cumulative += r;
    }

    /* Locate the row and position that contain the n-th term. */
    idx_t row = 1;
    idx_t terms_before = 0;
    while (terms_before + row < n) {
        terms_before += row;
        ++row;
    }

    idx_t pos = n - terms_before - 1;  /* zero-based index inside the row */
    return rows[row][pos];
}

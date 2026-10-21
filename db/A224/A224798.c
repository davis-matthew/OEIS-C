/*
 * A224798  Triangle T, read by rows, where the matrix square T^2 results in
 *          shifting T right one column to drop the secondary diagonal (which
 *          consists of powers of 2).
 * https://oeis.org/A224798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   * The sequence a(n) is the row‑wise traversal of a lower *
 *            triangular array T. The array is defined by: * T(i,i) = 1 *
 *            T(i,i-1) = 2^(i-1) (powers of two, the secondary diagonal) * For
 *            0 ≤ j ≤ i-2 * T(i,j) = 2 * T(i,j+1) * + Σ_{k=j+2}^{i-1} T(i,k) *
 *            T(k,j+1) * This relation follows from the proper...
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 44, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t **T = NULL;          /* triangular array, allocated lazily */
static size_t rows_allocated = 0;  /* number of rows currently stored */

/* Ensure that rows 0 … needed are computed and stored in T.  */
static void ensure_rows(idx_t needed)
{
    if (rows_allocated <= (size_t)needed) {
        size_t new_size = (size_t)(needed + 1);
        T = (seq_t**)realloc(T, new_size * sizeof(seq_t*));
        for (size_t i = rows_allocated; i <= needed; ++i) {
            T[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));
            if (i == 0) {
                T[i][0] = (seq_t)1;
                continue;
            }
            T[i][i]     = (seq_t)1;                    /* main diagonal */
            T[i][i-1]   = pow2((idx_t)(i-1));         /* secondary diagonal (powers of two) */
            for (int j = (int)i - 2; j >= 0; --j) {
                seq_t sum = 0;
                for (size_t k = (size_t)j + 2; k < i; ++k) {
                    sum += T[i][k] * T[k][j+1];
                }
                T[i][j] = (seq_t)2 * T[i][j+1] + sum;
            }
        }
        rows_allocated = new_size;
    }
}

seq_t a(idx_t n)
{
    /* locate the row that contains the n-th element */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* build rows up to r */
    ensure_rows(r);

    /* compute column index within the row */
    idx_t start = (idx_t)(r * (r + 1) / 2);
    idx_t col   = n - start;                   /* 0 ≤ col ≤ r */
    return T[(size_t)r][(size_t)col];
}

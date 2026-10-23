/*
 * A103238  Triangular matrix T, read by rows, that satisfies: T^2 + T =
 *          SHIFTUP(T), also T^(n+1) + T^n = SHIFTUP(T^n - D*T^(n-1)) for all
 *          n, where D is a diagonal matrix with diagonal(D) = diagonal(T) =
 *          {1,2,3,...}.
 * https://oeis.org/A103238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T[0][0] = 1;
   For i≥1: T[i][i] = i+1;
   For 0≤j<i: T[i][j] = T[i‑1][j] + Σ_{k=j}^{i‑1} T[i‑1][k] * T[k][j].
   Sequence a(n) is read by rows: a(n) = T[row_of(n)][n - row_of(n) * (row_of(n)+1) / 2];
*/
#include <stdlib.h>
#include "../utils/row_of.h"

seq_t a(idx_t n)
{
    /* Determine the row and column of the requested index */
    idx_t r = row_of(n);
    idx_t start = r * (r + 1) / 2;      /* index of the first element in row r */
    idx_t c = n - start;                /* column within the row */

    /* Allocate triangular array for rows 0..r */
    seq_t **T = (seq_t **)calloc((size_t)(r + 1), sizeof(seq_t *));
    if (!T) return (seq_t)0;  /* allocation failure */

    /* Row 0 */
    T[0] = (seq_t *)calloc(1, sizeof(seq_t));
    if (!T[0]) { free(T); return (seq_t)0; }
    T[0][0] = (seq_t)1;

    /* Build subsequent rows */
    for (idx_t i = 1; i <= r; ++i) {
        T[i] = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!T[i]) {
            /* Clean up before returning */
            for (idx_t k = 0; k < i; ++k) free(T[k]);
            free(T);
            return (seq_t)0;
        }
        T[i][i] = (seq_t)(i + 1);  /* diagonal entry */

        for (idx_t j = 0; j < i; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = j; k < i; ++k) {
                sum += T[i - 1][k] * T[k][j];
            }
            T[i][j] = T[i - 1][j] + sum;
        }
    }

    /* Retrieve the desired entry */
    seq_t result = T[r][c];

    /* Free allocated memory */
    for (idx_t i = 0; i <= r; ++i) free(T[i]);
    free(T);

    return result;
}

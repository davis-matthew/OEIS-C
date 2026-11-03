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
 * Validated: partial, pass, 46 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T[0][0] = 1;
   For i≥1: T[i][i] = i+1;
   For 0≤j<i: T[i][j] = T[i‑1][j] + Σ_{k=j}^{i‑1} T[i‑1][k] * T[k][j].
   Sequence a(n) is read by rows: a(n) = T[row_of(n)][n - row_of(n) * (row_of(n)+1) / 2];
*/
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

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
    T[0] = mem_new(seq_t, 1);
    if (!T[0]) { free(T); return (seq_t)0; }
    T[0][0] = (seq_t)1;

    /* Build subsequent rows */
    for (idx_t i = 1; i <= r; ++i) {
        T[i] = mem_new(seq_t, (size_t)(i + 1));
        if (!T[i]) {
            /* Clean up before returning */
            for (idx_t k = 0; k < i; ++k) mem_free(T[k]);
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
    for (idx_t i = 0; i <= r; ++i) mem_free(T[i]);
    free(T);

    return result;
}

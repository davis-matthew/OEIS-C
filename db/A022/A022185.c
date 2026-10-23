/*
 * A022185  Triangle of Gaussian binomial coefficients [ n,k ] for q = 21.
 * https://oeis.org/A022185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Gaussian binomial coefficients ⟦n,k⟧ with q = 21, flattened
 *            row‑wise. Recurrence for the triangle: T(0,0) = 1 T(n,0) = 1
 *            T(n,n) = 1 T(n,k) = T(n-1,k-1) + 21^k * T(n-1,k) (1 ≤ k ≤ n-1)
 *            The sequence a(n) is obtained by listing all T(n,k) row‑wise:
 *            a(n) = T(row, col) with row = floor( (sqrt(8n+1)-1)...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif
#include "../utils/mul_small.h"

/* Static cache for the rows of the triangle.
   rows[i] points to an array of length i+1, or NULL if not yet computed. */
static seq_t **rows = NULL;
static size_t rows_cap = 0;

/* Ensure that rows[0..r] are allocated and computed. */
static void
ensure_rows(size_t r)
{
    /* Allocate the array of pointers if necessary. */
    if (rows_cap == 0) {
        rows_cap = 1;
#if defined(__cplusplus)
        rows = new seq_t*[rows_cap];
#else
        rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
        if (!rows) abort();
#endif
        for (size_t i = 0; i < rows_cap; ++i) rows[i] = NULL;
    }

    /* Grow the pointer array if r is beyond current capacity. */
    if (rows_cap <= r) {
        size_t new_cap = rows_cap;
        while (new_cap <= r) new_cap <<= 1;
#if defined(__cplusplus)
        seq_t **tmp = new seq_t*[new_cap];
#else
        seq_t **tmp = (seq_t**)malloc(new_cap * sizeof(seq_t*));
        if (!tmp) abort();
#endif
        for (size_t i = 0; i < rows_cap; ++i) tmp[i] = rows[i];
        for (size_t i = rows_cap; i < new_cap; ++i) tmp[i] = NULL;
#if defined(__cplusplus)
        delete[] rows;
#else
        free(rows);
#endif
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute missing rows up to r. */
    for (size_t i = 0; i <= r; ++i) {
        if (rows[i] != NULL) continue;              /* already computed */

        /* Allocate memory for this row. */
#if defined(__cplusplus)
        seq_t *cur = new seq_t[i + 1];
#else
        seq_t *cur = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!cur) abort();
#endif

        cur[0] = 1;                                 /* T(i,0) = 1 */
        seq_t pow = 1;                               /* 21^0 */

        for (size_t k = 1; k <= i; ++k) {
            pow = mul_small(pow, 21);                /* 21^k */
            if (k == i) {                           /* T(i,i) = 1 */
                cur[k] = 1;
            } else {
                /* T(i,k) = T(i-1,k-1) + 21^k * T(i-1,k) */
                cur[k] = rows[i - 1][k - 1] + pow * rows[i - 1][k];
            }
        }
        rows[i] = cur;
    }
}

/* Return the n‑th term of the sequence. */
seq_t
a(idx_t n)
{
    /* Determine row and column from the flattened index. */
    size_t N = (size_t)n;
    size_t r = 0;
    size_t cum = 0;
    while (cum + (r + 1) <= N) {
        cum += r + 1;
        ++r;
    }
    size_t k = N - cum;

    ensure_rows(r);
    return rows[r][k];
}

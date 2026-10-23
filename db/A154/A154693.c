/*
 * A154693  Triangle read by rows: T(n, k) = (2^(n-k) + 2^k)*A008292(n+1,
 *          k+1).
 * https://oeis.org/A154693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattened value of the triangle T(n,k) = (2^(n-k) + 2^k)
 *            * A008292(n+1,k+1), read by rows and concatenated
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 69, u64 n <= 177,
 *            u128 n <= 505, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"          /* pow2(idx_t exp) -> seq_t */

/* Cached Eulerian triangle rows (row i has elements 0..i). */
static seq_t **rows = NULL;      /* array of pointers to row data */
static size_t rows_len = 0;      /* number of rows stored so far (0..rows_len-1) */
static size_t rows_cap = 0;      /* allocated capacity of 'rows' array */

/* Ensure that row 'r' (0‑based) of the Eulerian numbers A008292 is stored.
   Newly created rows are allocated with calloc. */
static void ensure_row(size_t r)
{
    if (rows_len > r) return;          /* row already available */

    /* Grow the rows pointer array if necessary. */
    if (rows_cap <= r) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 4;
        while (new_cap <= r) new_cap *= 2;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute missing rows up to r. */
    for (size_t i = rows_len; i <= r; ++i) {
        seq_t *row = (seq_t*)calloc(i + 1, sizeof(seq_t));
        if (!row) return;           /* allocation failure – caller UB */

        if (i == 0) {               /* A008292(1,1) = 1 */
            row[0] = 1;
        } else {
            seq_t *prev = rows[i - 1];
            for (size_t j = 0; j <= i; ++j) {
                seq_t term1 = (seq_t)(j + 1) * (j <= i - 1 ? prev[j] : 0);
                seq_t term2 = (seq_t)(i + 1 - j) * (j > 0 ? prev[j - 1] : 0);
                row[j] = term1 + term2;
            }
        }
        rows[i] = row;
    }
    rows_len = r + 1;
}

/* Return the Eulerian number A008292(r+1,k+1) for 0‑based indices r,k. */
static seq_t eulerian_0(size_t r, size_t k)
{
    ensure_row(r);
    return rows[r][k];
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Determine which row and column contain the n‑th flattened entry. */
    size_t r = 0;
    size_t pos = 0;                 /* index of the first element of row r */
    while (pos + r + 1 <= (size_t)n) {
        pos += r + 1;
        ++r;
    }
    size_t k = (size_t)n - pos;     /* column index within the row */

    /* Retrieve the supporting Eulerian number. */
    seq_t e = eulerian_0(r, k);      /* A008292(r+1,k+1) */

    /* Compute (2^(r-k) + 2^k) * e. */
    seq_t base = pow2((idx_t)(r - k)) + pow2((idx_t)k);
    return base * e;
}

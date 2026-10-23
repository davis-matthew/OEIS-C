/*
 * A119732  f-Pascal's triangle where f(n) = n*(n+1)/2 = A000217(n).
 * https://oeis.org/A119732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑major read of f‑Pascal's triangle, where f(x) =
 *            x*(x+1)/2, with T(0,0)=1 and T(n,k) = f(T(n‑1,k‑1)) +
 *            f(T(n‑1,k))
 * Range:     u8 n <= 16, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"

static inline seq_t f_func(seq_t x) { return x * ((seq_t)x + 1) / 2; }

/* static storage for triangle rows */
static seq_t **rows = NULL;      /* array of pointers to rows */
static size_t rows_capacity = 0; /* size of rows array */
static size_t rows_count = 0;    /* number of rows already computed */

/* Ensure rows 0..up_to are computed */
static void ensure_rows(idx_t up_to)
{
    /* grow the rows array if needed */
    if ((size_t)up_to + 1 > rows_capacity) {
        size_t new_cap = ((size_t)up_to + 1) * 2;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        rows = tmp;
        rows_capacity = new_cap;
    }

    /* compute any missing rows */
    for (idx_t r = (idx_t)rows_count; r <= up_to; ++r) {
        size_t len = (size_t)(r + 1);
        seq_t *row = seq_alloc(len); /* properly constructed array for seq_t */
        if (!row) abort();

        if (r == 0) {
            row[0] = 1;
        } else {
            seq_t *prev = rows[(size_t)(r - 1)];
            for (size_t k = 0; k < len; ++k) {
                seq_t left  = 0;
                seq_t right = 0;
                if (k > 0)      left = f_func(prev[k - 1]);
                if (k < r)     right = f_func(prev[k]); /* prev has size r */
                row[k] = left + right;
            }
        }

        rows[(size_t)r] = row;
    }

    rows_count = (size_t)up_to + 1;
}

/* return the n-th term of the sequence (0‑based index) */
seq_t a(idx_t n)
{
    /* Find row r containing the n‑th element (row‑major order) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2; /* column within that row */

    /* Ensure all rows up to r are computed */
    ensure_rows(r);

    return rows[(size_t)r][(size_t)k];
}

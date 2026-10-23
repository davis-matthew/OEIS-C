/*
 * A178666  Irregular triangle read by rows in which row n gives expansion of
 *          the polynomial Product_{k=0..n} (1 + x^(2*k + 1)), n >= -1.
 * https://oeis.org/A178666
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1109, u16 n <= 4573, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   prefix_rowsum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A178666: irregular triangle read by rows.
 * a(n) is the coefficient of x^k in
 *    Π_{t=0..r} (1 + x^(2t+1)),
 * where n maps to the row r and position k within that row.
 * Offset is –1: a(-1)=1.
 *
 * The function builds rows on demand and memoises them.
 * It works for any seq_t (POD or arbitrary‑precision) and idx_t
 * (int64_t) in C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/prefix_rowsum.h"

/* memoised rows: row_coeffs[0] is row 0, etc. */
static seq_t **row_coeffs   = NULL;
static size_t   rows_allocated = 0; /* number of rows stored (≥0) */
static size_t   rows_capacity  = 0; /* current capacity of row_coeffs */

/* ensure that rows 0 … target are built and stored */
static void ensure_rows(idx_t target)
{
    if (target < 0)
        return;   /* nothing to do for row –1 */

    for (idx_t rr = rows_allocated; rr <= target; ++rr) {
        /* length of row rr: (rr+1)^2 + 1,  rr ≥ 0 */
        idx_t len_cur = (rr + 1) * (rr + 1) + 1;

        /* allocate array for this row, zero‑initialised */
#if defined(__cplusplus)
        seq_t *new_row = new seq_t[len_cur]();   /* value‑init → 0 for POD, default ctor for class */
#else
        seq_t *new_row = calloc((size_t)len_cur, sizeof(seq_t));
        if (!new_row)
            abort();
#endif

        if (rr == 0) {
            /* base row: 1 + x^1  */
            new_row[0] = (seq_t)1;
            new_row[1] = (seq_t)1;
        } else {
            /* derived from row (rr‑1) */
            seq_t *prev = row_coeffs[rr - 1];
            idx_t len_prev = rr * rr + 1;            /* length of previous row: rr^2 + 1 */
            idx_t shift = 2 * rr + 1;               /* exponent shift from multiplying by (1 + x^(2rr+1)) */

            for (idx_t i = 0; i < len_prev; ++i) {
                new_row[i] += prev[i];               /* factor '1' */
                new_row[i + shift] += prev[i];       /* factor x^(2rr+1) */
            }
        }

        /* grow the row_coeffs array if necessary */
        if ((size_t)rr >= rows_capacity) {
            size_t new_cap = rows_capacity ? rows_capacity * 2 : 4;
            while (new_cap <= (size_t)rr)
                new_cap <<= 1;
            seq_t **tmp = (seq_t**)realloc(row_coeffs,
                                            new_cap * sizeof(seq_t*));
            if (!tmp)
                abort();
            row_coeffs = tmp;
            rows_capacity = new_cap;
        }

        row_coeffs[(size_t)rr] = new_row;
        ++rows_allocated;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* linear index p = n + 1, where row –1 starts at p = 0 */
    idx_t p = n + 1;
    if (p < 0)          /* negative indices are undefined */
        return (seq_t)0;

    /* find row r containing index p */
    idx_t r = -1;
    while (prefix_rowsum(r + 1) <= p)
        ++r;

    /* offset inside the found row */
    idx_t offset = p - prefix_rowsum(r);

    if (r == -1) {
        /* row –1 : single coefficient 1 */
        return (seq_t)1;
    }

    /* ensure required rows are built */
    ensure_rows(r);

    /* return the coefficient */
    return row_coeffs[(size_t)r][(size_t)offset];
}

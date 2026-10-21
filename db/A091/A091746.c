/*
 * A091746  Generalized Stirling2 array (6,2).
 * https://oeis.org/A091746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = row‑wise enumeration of the generalized Stirling‑2 array
 *            (r = 6, s = 2). The array entry a(n,k) satisfies the recurrence
 *            a(1,2) = 1, a(n,k) = Σ_{p=0..2} binom(2,p) ·
 *            fallfac(4·(n-1)+k‑p, 2‑p) · a(n‑1, k‑p) for n ≥ 2, 2 ≤ k ≤ 2n
 *            (here fallfac(m,0)=1, fallfac(m,1)=m, fallfac(m,2)=m·(m‑1)).
 *            T...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   integer_sqrt_ceil
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/integer_sqrt_ceil.h"

/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* storage for the computed rows                                 */
static seq_t **rows   = NULL;      /* rows[1 .. max_row] */
static idx_t  rows_cap = 0;        /* allocated capacity of 'rows' */
static idx_t  max_row   = 0;        /* largest row computed so far */

/* Ensure that rows 1 .. row_needed are computed and stored. */
static void ensure_rows(idx_t row_needed)
{
    while (row_needed > max_row)
    {
        /* create new row */
        idx_t const new_row = max_row + 1;
        size_t const len = 2 * new_row - 1;   /* number of k‑values: k=2..2n  */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        if (new_row == 1)
        {
            /* a(1,2) = 1 */
            row[0] = (seq_t)1;
        }
        else
        {
            /* use the recurrence to fill this row */
            idx_t const prev_row   = new_row - 1;
            idx_t const prev_max_k = 2 * prev_row;
            seq_t *const prev = rows[prev_row];

            for (idx_t k = 2; k <= 2 * new_row; ++k)
            {
                seq_t val = 0;

                /* p = 0 */
                if (k <= prev_max_k) {
                    seq_t t = (seq_t)(4 * prev_row + k);
                    seq_t fall = t * (t - 1);        /* fallfac(m,2) */
                    val += prev[k - 2] * fall;       /* binom(2,0)=1 */
                }

                /* p = 1 */
                if (k >= 1 && k - 1 >= 2 && k - 1 <= prev_max_k) {
                    seq_t t = (seq_t)(4 * prev_row + k - 1);
                    seq_t fall = t;                 /* fallfac(m,1) */
                    val += prev[(k - 1) - 2] * fall * 2; /* binom(2,1)=2 */
                }

                /* p = 2 */
                if (k >= 2 && k - 2 >= 2 && k - 2 <= prev_max_k) {
                    val += prev[(k - 2) - 2];      /* fallfac(m,0)=1, binom(2,2)=1 */
                }

                row[k - 2] = val;
            }
        }

        /* grow the array of row pointers if necessary */
        if (new_row > rows_cap) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 1;
            while (new_row > new_cap) new_cap *= 2;
            seq_t **tmp = (seq_t **)realloc(rows, (new_cap + 1) * sizeof(seq_t *));
            if (!tmp) exit(EXIT_FAILURE);
            rows = tmp;
            rows_cap = new_cap;
        }

        rows[new_row] = row;
        max_row = new_row;
    }
}

/*------------------------------------------------------------*/
/* requested function                                              */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, but harmless */

    /* find the row containing the index n */
    idx_t const row = integer_sqrt_ceil(n);
    ensure_rows(row);

    /* offset within the row (0‑based) */
    idx_t const offset = n - ((row - 1) * (row - 1)) - 1;

    return rows[row][offset];
}

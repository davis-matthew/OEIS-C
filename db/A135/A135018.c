/*
 * A135018  Triangle read by rows: row 1 = {1}, row 2 = {1,2}. Row n consists
 *          of the smallest n distinct positive integers such that no term in
 *          row (n-1) is the average of a term in row (n-2) and a term in row
 *          n.
 * https://oeis.org/A135018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2999, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135018: Triangle read by rows.
   The code constructs the rows iteratively.
   Each row n consists of the smallest n distinct positive integers such that
   no term in row (n‑1) is the average of a term in row (n‑2) and a term in row n. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t **rows = NULL;          /* array of pointers to each computed row */
static size_t rows_capacity = 0;     /* current capacity of the rows array */
static size_t rows_count = 0;        /* number of rows that have been computed */

/* Ensure that rows 1..r (1‑based) are available. */
static void ensure_rows_up_to(idx_t r)
{
    if (r < 1) return;

    /* One‑time initialization: row 1 and row 2 */
    if (rows_count == 0) {
        /* Start with capacity for 4 rows */
        rows_capacity = 4;
        rows = (seq_t **)malloc(rows_capacity * sizeof(seq_t *));

        /* Row 1 = {1} */
        rows[0] = (seq_t *)malloc(sizeof(seq_t));
        rows[0][0] = (seq_t)1;
        rows_count = 1;

        /* Row 2 = {1,2} */
        if (rows_count >= rows_capacity) {
            rows_capacity *= 2;
            rows = (seq_t **)realloc(rows, rows_capacity * sizeof(seq_t *));
        }
        rows[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        rows[1][0] = (seq_t)1;
        rows[1][1] = (seq_t)2;
        rows_count = 2;
    }

    /* Generate missing rows up to r */
    while (rows_count < (size_t)r) {
        size_t new_row_len = rows_count + 1;                /* length of this new row (row number) */
        seq_t *new_row = (seq_t *)malloc(new_row_len * sizeof(seq_t));

        const seq_t *prevprev = (rows_count >= 2) ? rows[rows_count - 2] : NULL;
        size_t  prevprev_len = (rows_count >= 2) ? (rows_count - 1) : 0; /* =r‑2 */
        const seq_t *prev     = rows[rows_count - 1];
        size_t  prev_len      = rows_count;                      /* =r‑1 */

        size_t filled = 0;
        seq_t cand = 1;                                        /* candidate value */

        while (filled < new_row_len) {
            bool bad = false;

            if (prevprev_len > 0) {
                for (size_t i = 0; i < prev_len && !bad; ++i) {
                    seq_t two_a = prev[i] * (seq_t)2;
                    if (two_a > cand) {                /* only then 2*a - cand > 0 */
                        seq_t x = two_a - cand;         /* 2*a - cand */
                        /* check if x belongs to prevprev */
                        for (size_t j = 0; j < prevprev_len; ++j) {
                            if (prevprev[j] == x) {
                                bad = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (!bad) {
                new_row[filled++] = cand;
            }
            ++cand;
        }

        /* Append new row to the rows array */
        if (rows_count >= rows_capacity) {
            rows_capacity *= 2;
            rows = (seq_t **)realloc(rows, rows_capacity * sizeof(seq_t *));
        }
        rows[rows_count] = new_row;
        ++rows_count;
    }
}

/* Return a(n) for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Find row r that contains index n. */
    idx_t r = 1;
    while (((r * (r + 1)) >> 1) < n) ++r;   /* r*(r+1)/2 < n */

    /* Ensure all rows up to r are computed. */
    ensure_rows_up_to(r);

    /* Starting index of row r (1‑based) */
    idx_t start = (((r - 1) * r) >> 1) + 1;
    idx_t pos = n - start;                /* 0‑based position within the row */

    return rows[r - 1][pos];
}

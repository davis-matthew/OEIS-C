/*
 * A054589  Table related to labeled rooted trees, cycles and binary trees.
 * https://oeis.org/A054589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle coefficients of polynomials pₙ defined by
   p₁ = 1
   pₙ₊₁ = x²·(dpₙ/dx) + n·(1 + x)·pₙ.
   The sequence a(n) lists these coefficients row‑wise:
   first the coefficients of p₁, then those of p₂, then p₃, ….
   For index n, find row r with r·(r+1)/2 ≥ n > (r−1)·r/2,
   column k = n − (r−1)·r/2 − 1, and return the coefficient
   a[r][k]. */
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    static seq_t **rows = NULL;
    static size_t rows_used = 0;   /* highest computed row (≥1) */
    static size_t rows_cap  = 0;   /* capacity of rows array */

    /* ----- find row r for index n ----- */
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * (double)n) - 1.0) / 2.0);
    while ((seq_t)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && ((seq_t)(r - 1) * r / 2) >= n) --r;

    /* ----- initialise first row if needed ----- */
    if (rows_used == 0) {
        if (rows_cap < 2) {
            rows_cap = 2;
            rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
            if (!rows) exit(1);
        }
        rows[1] = (seq_t*)malloc(sizeof(seq_t));
        if (!rows[1]) exit(1);
        rows[1][0] = 1;
        rows_used = 1;
    }

    /* ----- generate subsequent rows up to r ----- */
    while (rows_used < (size_t)r) {
        size_t curr = rows_used;                 /* current row number (≥1) */
        seq_t *prev     = rows[curr];
        size_t prev_len = curr;                  /* prev has 'curr' coefficients */
        size_t new_len  = prev_len + 1;          /* next row length */

        /* ensure capacity for the new row pointer */
        if (rows_cap <= curr + 1) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 2;
            while (new_cap <= curr + 1) new_cap <<= 1;
            seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            if (!tmp) exit(1);
            rows = tmp;
            rows_cap = new_cap;
        }

        seq_t *next = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!next) exit(1);

        seq_t rseq = (seq_t)curr;   /* n in the recurrence for pₙ₊₁ */

        /* coefficient for x⁰ */
        next[0] = rseq * prev[0];

        /* coefficients for x¹ … x⁽prev_len‑1⁾ */
        for (size_t j = 1; j < prev_len; ++j) {
            seq_t left  = (seq_t)(j - 1 + curr) * prev[j - 1];
            seq_t right = rseq * prev[j];
            next[j] = left + right;
        }

        /* coefficient for the highest degree term */
        next[new_len - 1] = (seq_t)(2 * curr - 1) * prev[prev_len - 1];

        rows[curr + 1] = next;
        ++rows_used;
    }

    /* ----- locate the desired coefficient ----- */
    idx_t prev_sum = (idx_t)(r - 1) * r / 2;   /* elements before row r */
    idx_t k = n - prev_sum - 1;                /* 0‑based column index */

    return rows[r][k];
}

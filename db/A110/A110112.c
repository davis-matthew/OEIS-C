/*
 * A110112  Square array of numbers associated to the recurrences b(k) =
 *          b(k-1) + n*b(k-2); array T(n,k), read by descending antidiagonals,
 *          for n, k >= 0.
 * https://oeis.org/A110112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A110112 – Square array read by descending antidiagonals.
 *
 *  Array T(n,k) is defined by:
 *      a(n,0) = 0, a(n,1) = 1
 *      a(n,k) = a(n,k-1) + n*a(n,k-2)   for k ≥ 2
 *      T(n,k) = a(n,k+1)*a(n,k+2)*a(n,k+3)/(n+1)
 *
 *  The sequence a(n) is obtained by enumerating T(n,k) along descending
 *  antidiagonals:  (0,0), (0,1),(1,0), (0,2),(1,1),(2,0), …
 *
 *  The function below computes a(n) for any non‑negative index n.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *table = NULL;     /* contiguous array of a(n,k) values   */
static size_t rows = 0;         /* number of rows currently stored     */
static size_t cols = 0;         /* number of columns currently stored  */

/*  Ensure that table has at least needed_rows rows and needed_cols columns.
 *  Recompute the whole table on each expansion – data size is small (≤200×200). */
static void ensure_table(size_t needed_rows, size_t needed_cols) {
    if (rows >= needed_rows && cols >= needed_cols)
        return;

    size_t r, c;
    seq_t *new_t = (seq_t *)malloc(needed_rows * needed_cols * sizeof(seq_t));
    if (!new_t)
        return; /* allocation failure – caller will get 0 */

    for (r = 0; r < needed_rows; ++r) {
        size_t base = r * needed_cols;
        new_t[base + 0] = (seq_t)0;              /* a(r,0) = 0 */
        if (needed_cols > 1)
            new_t[base + 1] = (seq_t)1;          /* a(r,1) = 1 */
        for (c = 2; c < needed_cols; ++c) {
            new_t[base + c] =
                new_t[base + c - 1] + (seq_t)r * new_t[base + c - 2];
        }
    }

    free(table);
    table = new_t;
    rows = needed_rows;
    cols = needed_cols;
}

/*  OEIS A110112 term a(n)                                         */
seq_t a(idx_t n) {
    /*  Find the antidiagonal m so that (m+1)(m+2)/2 > n ≥ m(m+1)/2 */
    size_t m = 0;
    idx_t idx = n;
    while (((idx_t)(m + 1) * (idx_t)(m + 2) / 2) <= idx)
        ++m;

    /*  Determine required table size                                            */
    size_t needed_rows = m + 1;    /* rows 0 .. m                       */
    size_t needed_cols = m + 4;    /* need indices up to k = m+3         */
    ensure_table(needed_rows, needed_cols);

    /*  Position inside the antidiagonal                                            */
    idx_t prev = (idx_t)(m * (m + 1) / 2);
    idx_t offset = idx - prev;                 /* 0 .. m */
    size_t row = (size_t)offset;
    size_t col = (size_t)(m - offset);          /* 0 .. m */

    /*  Compute T(row,col) = a(row,col+1)*a(row,col+2)*a(row,col+3)/(row+1) */
    seq_t v1 = table[row * cols + (col + 1)];
    seq_t v2 = table[row * cols + (col + 2)];
    seq_t v3 = table[row * cols + (col + 3)];
    seq_t prod = v1 * v2 * v3;
    seq_t result = prod / ((seq_t)(row + 1));

    return result;
}

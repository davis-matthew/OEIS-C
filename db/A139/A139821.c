/*
 * A139821  Triangle T(i,j) read by rows: T(i,1) = Fibonacci(i) for all i;
 *          T(i,i) = i for all i; T(i,j) = T(i-1,j) + T(i-2,j) + T(i-1,j-1) -
 *          T(i-2,j-1).
 * https://oeis.org/A139821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(i,j) defined by:
   T(i,1) = Fibonacci(i)
   T(i,i) = i
   T(i,j) = T(i-1,j) + T(i-2,j) + T(i-1,j-1) - T(i-2,j-1)
   (missing indices are treated as 0).
   a(n) is the triangle read by rows (offset 1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static state for memoisation ---------- */
    static seq_t **rows = NULL;        /* rows[1..max_i] -> array[1..i] */
    static idx_t *cum  = NULL;        /* cum[i] = 1+2+...+i */
    static seq_t *fibs = NULL;        /* Fibonacci values Fib(1..max_i) */
    static idx_t max_i = 0;           /* largest row already computed */

    static size_t rows_cap = 0;       /* allocated capacity for rows */
    static size_t cum_cap  = 0;       /* allocated capacity for cum  */
    static size_t fibs_cap = 0;       /* allocated capacity for fibs */

    /* ---------- initialise if needed ---------- */
    if (max_i == 0) {
        size_t init_cap = 8;
        rows = (seq_t **)malloc(init_cap * sizeof(seq_t *));
        cum  = (idx_t  *)malloc(init_cap * sizeof(idx_t ));
        fibs = (seq_t  *)malloc(init_cap * sizeof(seq_t ));
        if (!rows || !cum || !fibs) abort();
        rows_cap = init_cap;
        cum_cap  = init_cap;
        fibs_cap = init_cap;
        cum[0] = 0;          /* cumulative count before row 1 */
    }

    /* ---------- find target row ---------- */
    idx_t target_row = max_i;
    while ((target_row * (target_row + 1)) / 2 < n) {
        ++target_row;
    }

    /* ---------- grow storage if necessary ---------- */
    if (target_row >= rows_cap) {
        size_t new_cap = rows_cap == 0 ? 8 : rows_cap;
        while (new_cap <= target_row) new_cap <<= 1;
        seq_t **tmp_rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp_rows) abort();
        rows = tmp_rows;
        rows_cap = new_cap;
    }
    if (target_row >= cum_cap) {
        size_t new_cap = cum_cap == 0 ? 8 : cum_cap;
        while (new_cap <= target_row) new_cap <<= 1;
        idx_t *tmp_cum = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
        if (!tmp_cum) abort();
        cum = tmp_cum;
        cum_cap = new_cap;
    }
    if (target_row >= fibs_cap) {
        size_t new_cap = fibs_cap == 0 ? 8 : fibs_cap;
        while (new_cap <= target_row) new_cap <<= 1;
        seq_t *tmp_fibs = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
        if (!tmp_fibs) abort();
        fibs = tmp_fibs;
        fibs_cap = new_cap;
    }

    /* ---------- compute rows up to target_row ---------- */
    for (idx_t i = max_i + 1; i <= target_row; ++i) {

        /* allocate memory for row i (index 0 unused) */
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row) abort();
        rows[i] = row;

        /* compute Fibonacci(i) */
        seq_t fib_i;
        if (i == 1)
            fib_i = (seq_t)1;
        else if (i == 2)
            fib_i = (seq_t)1;
        else
            fib_i = fibs[i - 1] + fibs[i - 2];
        fibs[i] = fib_i;        /* store for future use */

        /* set border values */
        row[1] = fib_i;                 /* T(i,1) */
        row[i] = (seq_t)i;              /* T(i,i) */

        /* interior entries */
        for (idx_t j = 2; j <= i - 1; ++j) {
            seq_t a = rows[i - 1][j];
            seq_t b = (j <= i - 2) ? rows[i - 2][j] : (seq_t)0;
            seq_t c = rows[i - 1][j - 1];
            seq_t d = (j - 1 <= i - 2) ? rows[i - 2][j - 1] : (seq_t)0;
            row[j] = a + b + c - d;
        }

        /* update cumulative count */
        cum[i] = cum[i - 1] + i;
    }

    max_i = target_row;

    /* ---------- locate row and column for n ---------- */
    idx_t r = 1;
    while (cum[r] < n) ++r;
    idx_t j = n - cum[r - 1];
    return rows[r][j];
}

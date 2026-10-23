/*
 * A135685  Triangular sequence of the coefficients of the numerator of the
 *          rational recursive sequence for tan(n*y) with x = tan(y).
 * https://oeis.org/A135685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign
 * Formula:   a(n) = 0 for all odd n. For even n ≥ 2, let i = n/2 - 1; then
 *            a(n) = b[i], where b[0],b[1],… is the list of all non‑zero
 *            coefficients of the triangular array T defined by: T(0,0) = 0 ,
 *            T(1,0) = 0 , T(1,1) = 1 , for n ≥ 2: T(n,j) = -2*(-1)^n *
 *            T(n-1,j) + ( T(n-2,j) + (j≥2 ? T(n-2,j-2) : 0) ). The...
 * Range:     i8 n <= 55, i16 n <= 171, i32 n <= 595, i64 n <= 2275,
 *            i128 n <= 8643, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static data for memoisation */
static seq_t **rows = NULL;          /* array of pointers to rows of T   */
static idx_t  rows_size = 0;         /* number of rows currently stored */
static size_t rows_cap = 0;          /* capacity of rows array           */

static seq_t *B = NULL;              /* array of non‑zero coefficients   */
static idx_t  B_count = 0;           /* number of coefficients stored    */
static size_t B_cap = 0;             /* capacity of B array              */

/* append a new coefficient to B, extending capacity if needed */
static void append_B(seq_t val)
{
    if (B_count == B_cap) {
        size_t new_cap = (B_cap == 0) ? 16 : B_cap * 2;
        seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        B = tmp;
        B_cap = new_cap;
    }
    B[B_count++] = val;
}

/* ensure that rows[0..r] exist & B has at least needed entries */
static void ensure_B_to(idx_t needed)
{
    /* initialise rows array with row 0 and row 1 if not yet done */
    if (rows_size == 0) {
        rows_cap = 4;
        rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        if (!rows) abort();
        /* row 0: length 1, T(0,0)=0 */
        seq_t *row0 = (seq_t *)calloc(1, sizeof(seq_t));
        row0[0] = (seq_t)0;
        rows[0] = row0;
        rows_size = 1;
    }
    if (rows_size == 1) {
        /* row 1: length 2, T(1,0)=0, T(1,1)=1 */
        seq_t *row1 = (seq_t *)calloc(2, sizeof(seq_t));
        row1[0] = (seq_t)0;
        row1[1] = (seq_t)1;
        if (rows_size == rows_cap) {
            rows_cap *= 2;
            seq_t **tmp = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
            if (!tmp) abort();
            rows = tmp;
        }
        rows[1] = row1;
        rows_size = 2;
        /* row1's non‑zero coefficient is 1 */
        append_B((seq_t)1);
    }

    /* grow until we have enough non‑zero coefficients */
    while (B_count <= needed) {
        idx_t n = rows_size;               /* index of the new row to compute */
        if (rows_size == rows_cap) {
            rows_cap *= 2;
            seq_t **tmp = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
            if (!tmp) abort();
            rows = tmp;
        }
        size_t len = n + 1;
        seq_t *new_row = (seq_t *)calloc(len, sizeof(seq_t));
        if (!new_row) abort();

        /* compute sign = -2*(-1)^n   */
        seq_t sign = ((n & 1) ? (seq_t)2 : (seq_t)-2);

        for (idx_t j = 0; j < len; ++j) {
            seq_t term1 = 0;
            if (j <= n - 1)                /* T(n-1,j) exists */
                term1 = sign * rows[n - 1][j];

            seq_t term2 = 0;
            if (j <= n - 2)                /* T(n-2,j) exists */
                term2 += rows[n - 2][j];
            if (j >= 2)                    /* T(n-2,j-2) exists */
                term2 += rows[n - 2][j - 2];

            new_row[j] = term1 + term2;
        }

        rows[rows_size++] = new_row;

        /* append non‑zero coefficients to B */
        for (idx_t j = 0; j < len; ++j)
            if (new_row[j] != 0)
                append_B(new_row[j]);
    }
}

/* external function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if ((n & 1U) != 0) /* odd index */
        return (seq_t)0;

    idx_t idx = n / 2 - 1;          /* index into B */
    ensure_B_to(idx);
    return B[idx];
}

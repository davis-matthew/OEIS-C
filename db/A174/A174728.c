/*
 * A174728  Triangle read by rows: T(n, m, q) = (1-q^n)*Eulerian(n+1, m) -
 *          (1-q^n) + 1, with q = 2.
 * https://oeis.org/A174728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n, m, 2) = (1 - 2^n) * Eulerian(n+1, m) - (1 - 2^n) + 1 The
 *            sequence a(k) is defined by reading the triangle row‑wise (n =
 *            floor((sqrt(8k+1)-1)/2), m = k - n*(n+1)/2). a(k) = T(n, m, 2).
 *            This implementation memoizes Eulerian numbers row by row and
 *            uses simple arithmetic. It works for any seq_t,...
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 57, i64 n <= 158,
 *            i128 n <= 417, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- Eulerian number table (row → pointer to row array) ---------- */

static seq_t **eul_rows = NULL;   /* array of pointers to rows of Eulerian numbers */
static idx_t eul_rows_cnt = 0;    /* number of rows currently computed (≥ 1 means row 0..n-1 exist) */

/* Allocate a row of length len (len >= 1).  Row i has len = i+1. */
static inline seq_t *alloc_row(idx_t len) {
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t *)malloc(sizeof(seq_t) * len);
#endif
}

/* Ensure that rows 0 .. up_to (inclusive) are available.  up_to >= 0. */
static void ensure_eul_rows(idx_t up_to)
{
    if (eul_rows_cnt > up_to)          /* already have enough rows */
        return;

    /* Resize the row pointer array if needed */
#ifdef __cplusplus
    seq_t **tmp = new seq_t *[up_to + 1];
    for (idx_t i = 0; i < eul_rows_cnt; ++i)
        tmp[i] = eul_rows[i];
    delete[] eul_rows;
    eul_rows = tmp;
#else
    eul_rows = (seq_t **)realloc(eul_rows, sizeof(*eul_rows) * (up_to + 1));
#endif

    /* Compute new rows */
    for (idx_t i = eul_rows_cnt; i <= up_to; ++i) {
        eul_rows[i] = alloc_row(i + 1);   /* row i has indices 0..i */

        if (i == 0) {                         /* Eulerian(0,0) = 1 */
            eul_rows[i][0] = (seq_t)1;
            continue;
        }

        for (idx_t k = 0; k <= i; ++k) {
            seq_t val = 0;

            /* (k+1) * E(i-1, k)  if k <= i-1 */
            if (k <= i - 1)
                val += eul_rows[i - 1][k] * ((seq_t)(k + 1));

            /* (i-k) * E(i-1, k-1)  if k > 0 */
            if (k > 0)
                val += eul_rows[i - 1][k - 1] * ((seq_t)(i - k));

            eul_rows[i][k] = val;
        }
    }

    eul_rows_cnt = up_to + 1;   /* rows 0 .. up_to inclusive are computed */
}

/* ---------- Sequence function ---------- */

seq_t a(idx_t n)
{
    /* Find the row r and column m such that the triangle element
       T(r, m, 2) equals a(n).  The triangle has r+1 elements in row r,
       so the cumulative counts are r*(r+1)/2 up to the start of row r.  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)   /* (r+1)*(r+2)/2 = triangular(r+1) */
        ++r;

    idx_t before_row_start = r * (r + 1) / 2;   /* number of terms before row r */
    idx_t m = n - before_row_start;            /* column inside the row (0‑based) */

    /* Make sure the Eulerian numbers for row r+1 are computed,
       because we need Eulerian(n+1, m) = Eulerian(r+1, m). */
    ensure_eul_rows(r + 1);

    seq_t E = eul_rows[r + 1][m];          /* Eulerian(r+1, m) */

    /* 2^r as a seq_t: for arbitrary precision types this works as
       long as << is defined (most big‑int libraries overload it). */
    seq_t two_pow = ((seq_t)1) << r;        /* 2^r */

    /* (1 - 2^r) */
    seq_t coeff = (seq_t)1 - two_pow;

    /* (E - 1) */
    seq_t e_minus_1 = E - (seq_t)1;

    /* T(r, m, 2) = coeff * (E-1) + 1 */
    seq_t result = coeff * e_minus_1 + (seq_t)1;

    return result;
}

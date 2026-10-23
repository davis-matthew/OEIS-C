/*
 * A157832  Triangle read by rows: the coefficient [x^k] of the polynomial
 *          Product_{i=1..n} (5^(i-1)-x) in row n, column k, 0 <= k <= n.
 * https://oeis.org/A157832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient of x^k in the product \prod_{i=1}^{r}
 *            (5^{i-1} - x) read row by row. Here r and k are determined by
 *            the triangular indexing of n: r(r+1)/2 <= n < (r+1)(r+2)/2, k =
 *            n - r(r+1)/2. The recurrence used is T(0,0)=1,
 *            T(r,0)=5^{r-1}·T(r-1,0) T(r,k)=5^{r-1}·T(r-1,k)-T(r-1,k-1)
 *            for...
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 20, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global cache of rows and powers of 5.  */
static seq_t **rows = NULL;      /* rows[0] .. rows[r] are pointers to arrays of length r+1 */
static idx_t   rows_len = 0;     /* number of rows currently stored */
static idx_t   rows_cap = 0;     /* capacity allocated for rows array */

static seq_t *pow5 = NULL;       /* pow5[i] = 5^i */
static idx_t  pow5_cap = 0;      /* capacity allocated for pow5 array */

/* Ensure space for at least new_cap pointers in rows. */
static void ensure_rows_capacity(idx_t new_cap) {
    if (new_cap <= rows_cap) return;
    idx_t new_size = rows_cap ? rows_cap * 2 : 4;
    if (new_size < new_cap) new_size = new_cap;
    rows = (seq_t **)realloc(rows, new_size * sizeof(seq_t *));
    rows_cap = new_size;
}

/* Ensure space for at least new_cap seq_t values in pow5. */
static void ensure_pow5_capacity(idx_t new_cap) {
    if (new_cap <= pow5_cap) return;
    idx_t new_size = pow5_cap ? pow5_cap * 2 : 4;
    if (new_size < new_cap) new_size = new_cap;
    pow5 = (seq_t *)realloc(pow5, new_size * sizeof(seq_t));
    pow5_cap = new_size;
}

/* Build all rows up to and including row 'up_to'. */
static void compute_rows_up_to(idx_t up_to) {
    while (rows_len <= up_to) {
        idx_t cur = rows_len;   /* index of new row to build */
        ensure_rows_capacity(rows_len + 1);

        /* Allocate storage for the new row. */
        seq_t *row = (seq_t *)malloc((cur + 1) * sizeof(seq_t));
        if (cur == 0) {
            row[0] = (seq_t)1;   /* base case */
        } else {
            seq_t *prev = rows[cur - 1];
            seq_t p5 = pow5[cur - 1];  /* 5^{cur-1} */
            for (idx_t k = 0; k <= cur; ++k) {
                seq_t prev_val   = (k <= cur - 1)   ? prev[k]      : (seq_t)0;
                seq_t prev_minus = (k > 0)          ? prev[k - 1]  : (seq_t)0;
                row[k] = p5 * prev_val - prev_minus;
            }
        }
        rows[rows_len] = row;

        /* Update powers of 5. */
        ensure_pow5_capacity(cur + 1);
        if (cur == 0) {
            pow5[0] = (seq_t)1;
        } else {
            pow5[cur] = pow5[cur - 1] * (seq_t)5;
        }
        ++rows_len;
    }
}

/* External function required by the framework. */
seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;                          /* column within the row */
    compute_rows_up_to(r);                          /* ensure needed rows exist */
    return rows[r][k];
}

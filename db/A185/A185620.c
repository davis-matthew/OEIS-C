/*
 * A185620  Triangular matrix T that satisfies: T^3 - T^2 + I = SHIFT_LEFT(T),
 *          as read by rows.
 * https://oeis.org/A185620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 45, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A185620 – Triangular matrix T with recurrence:
 *   T(n+1,k+1) = [T³](n,k) – [T²](n,k) + [T⁰](n,k)
 *   with T(n,0)=1 for all n≥0.
 * The sequence a(n) is the nth entry of T read row‑wise (offset 0).
 *
 * Implementation builds the triangle lazily, memoising all rows
 * up to the required one.  For n≈10⁴ only about 140 rows are needed,
 * so the algorithm runs in well under the time limit.
 */

#include <stdlib.h>
#include <math.h>

static seq_t **rows = NULL;          /* pointers to rows of T        */
static seq_t **conv2_rows = NULL;    /* convolution squares          */
static seq_t **conv3_rows = NULL;    /* convolution cubes            */
static size_t rows_cap = 0;          /* allocated capacity for rows  */
static size_t rows_count = 0;        /* number of rows built          */

/* Ensure the row arrays have capacity for at least 'need' rows */
static void ensure_rows_capacity(size_t need) {
    if (rows_cap >= need) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;

    seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
    if (!tmp) abort(); rows = tmp;

    tmp = (seq_t**)realloc(conv2_rows, new_cap * sizeof(seq_t*));
    if (!tmp) abort(); conv2_rows = tmp;

    tmp = (seq_t**)realloc(conv3_rows, new_cap * sizeof(seq_t*));
    if (!tmp) abort(); conv3_rows = tmp;

    rows_cap = new_cap;
}

/* Initialise row 0 and its convolution tables */
static void init_row_0(void) {
    rows[0] = (seq_t*)malloc(sizeof(seq_t));
    if (!rows[0]) abort();
    rows[0][0] = (seq_t)1;

    conv2_rows[0] = (seq_t*)malloc(sizeof(seq_t));
    if (!conv2_rows[0]) abort();
    conv2_rows[0][0] = rows[0][0] * rows[0][0];   /* [T²](0,0) */

    conv3_rows[0] = (seq_t*)malloc(sizeof(seq_t));
    if (!conv3_rows[0]) abort();
    conv3_rows[0][0] = rows[0][0] * conv2_rows[0][0]; /* [T³](0,0) */

    rows_count = 1;
}

/* Build row r (r ≥ 1) using the recurrence and store its convolution tables */
static void build_row(size_t r) {
    ensure_rows_capacity(r + 1);

    seq_t *row = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!row) abort();
    row[0] = (seq_t)1;  /* T(r,0) = 1 */

    /* Compute T(r,k+1) for k = 0 .. r-1 */
    for (size_t k = 0; k < r; ++k) {
        seq_t val = conv3_rows[r - 1][k] - conv2_rows[r - 1][k];
        if (r - 1 == k) val += (seq_t)1;   /* add the identity term */
        row[k + 1] = val;
    }
    rows[r] = row;

    /* Compute [T²](r,k) for all k */
    seq_t *conv2 = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!conv2) abort();
    for (size_t k = 0; k <= r; ++k) {
        seq_t sum = (seq_t)0;
        for (size_t t = k; t <= r; ++t)
            sum += rows[r][t] * rows[t][k];
        conv2[k] = sum;
    }
    conv2_rows[r] = conv2;

    /* Compute [T³](r,k) for all k */
    seq_t *conv3 = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!conv3) abort();
    for (size_t k = 0; k <= r; ++k) {
        seq_t sum = (seq_t)0;
        for (size_t p = k; p <= r; ++p)
            sum += rows[r][p] * conv2_rows[p][k];
        conv3[k] = sum;
    }
    conv3_rows[r] = conv3;

    rows_count = r + 1;
}

/* Build matrix rows up to at least 'r' */
static void build_up_to_row(size_t r) {
    if (rows_count == 0) {
        ensure_rows_capacity(4);
        init_row_0();
    }
    while (rows_count <= r)
        build_row(rows_count);
}

/* Convert linear index n to (row,col) in T */
static void index_to_row_col(idx_t n, size_t *prow, size_t *pcol) {
    /* Inverse triangular formula (using long double for safety) */
    long double dn = (long double)n;
    size_t row = (size_t)((sqrtl(8.0L * dn + 1.0L) - 1.0L) / 2.0L);
    /* Correct rounding errors */
    while ((unsigned __int128)(row + 1) * (row + 2) / 2 <= n) ++row;
    while ((unsigned __int128)row * (row + 1) / 2 > n) --row;
    unsigned __int128 tri = (unsigned __int128)row * (row + 1) / 2;
    size_t col = (size_t)(n - tri);
    *prow = row;
    *pcol = col;
}

/* Public function required by the build */
seq_t a(idx_t n) {
    size_t row, col;
    index_to_row_col(n, &row, &col);
    build_up_to_row(row);
    return rows[row][col];
}

/*
 * A121745  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n and having k columns of odd length (0<=k<=n). A deco
 *          polyomino is a directed column-convex polyomino in which the
 *          height, measured along the diagonal, is attained only in the last
 *          column.
 * https://oeis.org/A121745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Triangle P[n](t) = Q[n](t,1,t,1) with
 *   Q[0](t,s,x,y) = 1,
 *   Q[1](t,s,x,y) = x,
 *   Q[n](t,s,x,y) = Q[n-1](t,s,y,x) +
 *                    ( floor(n/2)*x + floor((n-1)/2)*y ) * Q[n-1](t,s,t,s)
 *   for n ≥ 2.
 *
 * The function below returns the flattened triangle element a(n);
 * a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

static seq_t **rows = NULL;   /* rows[i] points to array of length i+1 */
static size_t rows_size = 0;  /* number of rows currently allocated */

/* ensure rows[0..n] are computed */
static void ensure_rows(size_t n)
{
    if (rows_size == 0) {
        /* initialise row 0 and row 1 */
        rows_size = 2;
        rows = (seq_t**)malloc(rows_size * sizeof(seq_t*));
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        rows[0][0] = (seq_t)1;            /* T(0,0)=1 */

        rows[1] = (seq_t*)malloc(2 * sizeof(seq_t));
        rows[1][0] = (seq_t)0;            /* T(1,0)=0 */
        rows[1][1] = (seq_t)1;            /* T(1,1)=1 */
    }

    if (rows_size > n) return;            /* already enough */

    size_t old_size = rows_size;
    rows_size = n + 1;
    rows = (seq_t**)realloc(rows, rows_size * sizeof(seq_t*));

    /* compute missing rows */
    for (size_t r = old_size; r <= n; ++r) {
        if (r < 2) continue;            /* rows 0 and 1 already set */

        /* previous rows */
        seq_t *a = rows[r-1];           /* length r   */
        seq_t *b = rows[r-2];           /* length r-1 */

        /* allocate current row array of size r+1 */
        seq_t *c = (seq_t*)calloc(r+1, sizeof(seq_t));   /* initialise to 0 */

        /* helper constants */
        seq_t ff = (seq_t)((r-1)/2) + (seq_t)1;          /* (floor((r-1)/2)+1) */
        seq_t sh = (seq_t)((r-2)/2);                    /* floor((r-2)/2)   */
        seq_t c0 = (seq_t)((r-1)/2);                    /* floor((r-1)/2)   */
        seq_t c1 = (seq_t)(r/2);                        /* floor(r/2)      */

        /* part from row r-2 (R_{r-1}) */
        for (size_t i = 0; i <= r-2; ++i)    /* a[i] exists for i<=r-2 */
            c[i] += ff * b[i];
        for (size_t i = 1; i <= r-1; ++i)    /* shift term */
            c[i] += sh * b[i-1];

        /* term from row r-1 curving with a */
        for (size_t i = 0; i <= r-1; ++i)
            c[i] += c0 * a[i];
        for (size_t i = 1; i <= r; ++i)
            c[i] += c1 * a[i-1];

        rows[r] = c;
    }
}

/* flattened triangle index -> (row, col) */
static void index_to_rc(idx_t idx, idx_t *row, idx_t *col)
{
    /* compute row r: r*(r+1)/2 <= idx < (r+1)*(r+2)/2 */
    double d = sqrt(1.0 + 8.0 * (double)idx);
    idx_t r = (idx_t)((-1.0 + d) / 2.0);
    /* correct possible rounding errors */
    while ((r+1)*(r+2)/2 <= idx) ++r;
    while (r*(r+1)/2 > idx) --r;
    *row = r;
    *col = idx - r*(r+1)/2;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    index_to_rc(n, &r, &k);
    ensure_rows((size_t)r);
    return rows[r][k];
}

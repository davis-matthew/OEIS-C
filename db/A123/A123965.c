/*
 * A123965  Triangle read by rows: T(0,0)=1; T(n,k) is the coefficient of x^k
 *          in the polynomial (-1)^n*p(n,x), where p(n,x) is the
 *          characteristic polynomial of the n X n tridiagonal matrix with 3's
 *          on the main diagonal and -1's on the super- and subdiagonal (n >=
 *          1; 0 <= k <= n).
 * https://oeis.org/A123965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient of x^k in the characteristic polynomial
 *            * p(r,x) of an r×r tridiagonal matrix with 3 on the main
 *            diagonal and -1 * on the sub‑ and super‑diagonals. The
 *            polynomials satisfy * p(0,x) = 1, * p(1,x) = 3 - x, * p(r,x) =
 *            (3 - x)·p(r-1,x) - p(r-2,x) (r ≥ 2). * a(n) is obtained by...
 * Range:     i8 n <= 14, i16 n <= 46, i32 n <= 173, i64 n <= 637,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Intermediate type large enough for intermediate arithmetic.
 * Prefer 128‑bit if available; otherwise fall back to 64‑bit. */
#if defined(__SIZEOF_INT128__)
typedef __int128 big_t;
#else
typedef long long big_t;
#endif

/* Cache of coefficient rows.  rows[i] points to the i‑th row (length i+1). */
static seq_t **rows = NULL;      /* array of pointers to rows          */
static idx_t  rows_used = 0;     /* rows[0 .. rows_used-1] are valid   */
static idx_t  rows_cap  = 0;     /* current capacity of rows array     */

/* Ensure that rows[0 .. min_needed] exist and are stored. */
static void ensure_rows(idx_t min_needed)
{
    /* Grow the rows pointer array if necessary. */
    if (rows_cap < min_needed + 1) {
        size_t new_cap = rows_cap ? rows_cap : 4;
        while (new_cap < (size_t)(min_needed + 1))
            new_cap <<= 1;
        seq_t **new_ptr = (seq_t **)malloc(new_cap * sizeof(seq_t *));
        if (!new_ptr) abort();
        if (rows_used)
            memcpy(new_ptr, rows, rows_used * sizeof(seq_t *));
        free(rows);
        rows       = new_ptr;
        rows_cap   = (idx_t)new_cap;
    }

    /* Compute rows that are missing. */
    while (rows_used <= min_needed) {
        idx_t r = rows_used;
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!row) abort();

        if (r == 0) {
            row[0] = (seq_t)1;
        } else if (r == 1) {
            row[0] = (seq_t)3;
            row[1] = (seq_t)-1;
        } else {
            /* Recurrence:  p(r,x) = (3-x)p(r-1,x) – p(r-2,x) */
            for (idx_t k = 0; k <= r; ++k) {
                big_t a = 0, b = 0, c = 0;

                if (k <= r - 1)  a = (big_t)rows[r - 1][k];    /* T(r-1,k)   */
                if (k >= 1)      b = (big_t)rows[r - 1][k - 1];/* T(r-1,k-1) */
                if (k <= r - 2)  c = (big_t)rows[r - 2][k];    /* T(r-2,k)   */

                big_t val = 3 * a - b - c;                    /* T(r,k)     */
                row[k]    = (seq_t)val;                       /* truncate   */
            }
        }

        rows[rows_used] = row;
        ++rows_used;
    }
}

/* Map global index n to triangle coordinates (r,k).   */
static inline void index_to_rowk(idx_t n, idx_t *r, idx_t *k)
{
    idx_t rr = 0;
    /* Find largest r with r(r+1)/2 ≤ n. */
    while ((rr + 1) * (rr + 2) / 2 <= n) ++rr;
    *r = rr;
    *k = n - rr * (rr + 1) / 2;
}

/* Public function to return a(n). */
seq_t a(idx_t n)
{
    idx_t r, k;
    index_to_rowk(n, &r, &k);
    ensure_rows(r);
    return rows[r][k];
}

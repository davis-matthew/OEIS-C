/*
 * A154985  Triangle T(n, k, m) = coefficients of p(x, n, m) where p(x,n,m) =
 *          (x+1)*p(x, n-1, m) + (2^(m+n-1) + 2^(n-2)*[n>=3])*x*p(x, n-2, m)
 *          and m=1, read by rows.
 * https://oeis.org/A154985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 59, u64 n <= 126,
 *            u128 n <= 261, big any n
 * Helpers:   coeff_row, pow2_seqt, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A154985
    Triangle T(n,k,1) = coefficients of
        p(x,n,1) = (x+1)p(x,n-1,1) + (2^(n+1-1)+2^(n-2)[n≥3])x p(x,n-2,1),
    read row by row.  a(n) = T(offset_row(m), k) where
        offset_row(r) = r(r+1)/2, k = n - offset_row(r).
    The function stores all rows that have been constructed so far, so
    subsequent calls for arbitrary n are O(1) after the needed rows are
    built.  The implementation uses only integer arithmetic and big‑int
    operations that are provided by the build. */
#include <stdlib.h>
#include <string.h>

/* Declaration of the auxiliary power routine from the build.  It
   returns a seq_t containing base^exp.  */
#include "../utils/power5.h"   /* power5 just gives power of 2 or 5 */
#include "../utils/coeff_row.h"
#include "../utils/pow2_seqt.h"

/* ----------  storage for computed rows  ---------- */

static seq_t **rows = NULL;        /* array of pointers to rows */
static size_t rows_len = 0;        /* number of rows stored */
static size_t rows_cap = 0;        /* allocated capacity */

/* Ensure that rows[] has room for at least need rows. */
static void ensure_rows_cap(size_t need)
{
    if (rows_cap < need) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 1;
        if (new_cap < need)
            new_cap = need;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp)
            abort();
        rows = tmp;
        rows_cap = new_cap;
    }
}

/* Compute rows up to (and including) row r_needed.  Each row is
   stored in rows[i] which points to an array of length i+1. */
static void compute_rows_up_to(idx_t r_needed)
{
    if (rows_len == 0) {
        /* initialise first two rows */
        ensure_rows_cap(2);
        rows[0] = (seq_t *)malloc(sizeof(seq_t));
        rows[0][0] = (seq_t)1;
        rows_len = 1;

        rows[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        rows[1][0] = rows[1][1] = (seq_t)1;
        rows_len = 2;
    }

    while (rows_len <= r_needed) {
        idx_t r = (idx_t)rows_len;              /* index of new row   */
        size_t len = r + 1;                     /* number of elements */
        seq_t *new_row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!new_row)
            abort();

        /* first and last element are always 1 */
        new_row[0] = new_row[r] = (seq_t)1;

        seq_t m = coeff_row(r);                 /* coefficient for this row */

        /* use recurrence: T(r,k) = T(r-1,k) + T(r-1,k-1) + m*T(r-2,k-1) */
        for (idx_t k = 1; k < r; ++k) {
            seq_t t1 = rows[r - 1][k];          /* from row r-1, same k */
            seq_t t2 = rows[r - 1][k - 1];      /* from row r-1, k-1   */
            seq_t t3 = m * rows[r - 2][k - 1];  /* from row r-2, k-1   */
            new_row[k] = t1 + t2 + t3;
        }

        ensure_rows_cap(rows_len + 1);
        rows[rows_len++] = new_row;
    }
}

/* Knuth style binary search to locate row containing index n.
   Returns row r (0‑based) and puts column offset into *k. */
static void locate_row(idx_t n, idx_t *row, idx_t *k)
{
    idx_t low = 0, high = 0;
    /* Find upper bound for row */
    while ((high + 1) * (high + 2) / 2 <= n)
        high = high * 2 + 1;
    low = 0;
    while (low + 1 < high) {
        idx_t mid = (low + high) / 2;
        if ((mid + 1) * (mid + 2) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    *row = high;
    *k = n - high * (high + 1) / 2;
}

/* ------------  required public function  ------------ */

seq_t a(idx_t n)
{
    /* Find row / column for flat index n */
    idx_t r, k;
    locate_row(n, &r, &k);
    compute_rows_up_to(r);
    return rows[r][k];
}

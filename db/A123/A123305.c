/*
 * A123305  Triangle T, read by rows, where column k of T equals (k+1)*(column
 *          k of T^2) when shifted to have an initial '1'; i.e., T(n,k) =
 *          (k+1)*[T^2](n-1,k) for n>k>=0, with T(n,n)=1 for n>=0.
 * https://oeis.org/A123305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (k+1) * Σ_{j=0}^{n-1} T(n-1,j) * T(j,k) (n>0) * T(n,n)
 *            = 1 (n≥0) * The sequence a(n) is the triangle read by rows:
 *            a(0)=1, a(1)=1, a(2)=1, …
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes needed by both C11 and C++17 */
#ifndef NO_SEQ_T
#include <stdint.h>
#else
/* When seq_t is a C++ big-integer class, <stdint.h> may not be needed. */
#endif
#include <stdlib.h>

/* External integer types supplied by the build */
extern seq_t seq_zero(void);  /* optional helper returning zero of seq_t type */

/* Cached rows of the triangle (static storage for memoisation) */
static seq_t **rows       = NULL; /* pointer to array of row pointers */
static idx_t  rows_len    = 0;    /* number of rows currently stored (next to compute) */
static idx_t  rows_cap    = 0;    /* capacity of the rows array */

/* Cumulative number of elements up to and including row r
   C(r) = (r+1)(r+2)/2.  Uses 128‑bit cast to prevent overflow for r≤10⁶. */
static inline idx_t C(idx_t r)
{
    return (idx_t)(((__int128)(r + 1) * (r + 2)) / 2);
}

/* Ensure that all rows up to r (inclusive) are computed and stored. */
static void ensure_rows(idx_t r)
{
    if (rows_len > r) return;                /* already have them */

    /* Resize the rows array if necessary */
    if (rows_cap < r + 1) {
        idx_t new_cap = rows_cap ? rows_cap * 2 : 4;
        while (new_cap <= r) new_cap <<= 1;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) exit(1);                   /* allocation failed */
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute any missing rows */
    for (idx_t i = rows_len; i <= r; ++i) {
        /* Allocate row i with (i+1) entries */
        seq_t *row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
        if (!row) exit(1);                   /* allocation failed */
        rows[i] = row;

        if (i == 0) {                       /* base case: T(0,0)=1 */
            row[0] = (seq_t)1;
            continue;
        }

        seq_t *prev_row = rows[i - 1];       /* T(i-1,*) */

        /* Columns 0 .. i-1 */
        for (idx_t k = 0; k < i; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t j = k; j < i; ++j) { /* j must be ≥ k so that T(j,k) is defined */
                /* T(i-1,j) * T(j,k) */
                sum += prev_row[j] * rows[j][k];
            }
            /* Multiply by (k+1) */
            row[k] = sum * (seq_t)(k + 1);
        }

        /* Diagonal entry */
        row[i] = (seq_t)1;
    }
    rows_len = r + 1;
}

/* Main function to return the n-th term of sequence A123305. */
seq_t a(idx_t n)
{
    /* Find the row r such that C(r-1) ≤ n < C(r)  */
    idx_t low  = 0;
    idx_t high = n;                    /* high is exclusive */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (C(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;                   /* minimal r with C(r) > n */

    /* Column index within that row */
    idx_t prev_count = (row == 0) ? 0 : C(row - 1);
    idx_t col = n - prev_count;        /* 0-based column in the row */

    /* Compute rows up to 'row' if needed */
    ensure_rows(row);

    return rows[row][col];
}

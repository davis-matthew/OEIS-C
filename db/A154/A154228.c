/*
 * A154228  Triangle T(n, k) = T(n-1, k) + T(n-1, k-1) +
 *          ((n+1)*(n+2)*(2*n+3)/6)*T(n-2, k-1), read by rows.
 * https://oeis.org/A154228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 59, u64 n <= 143,
 *            u128 n <= 389, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154228 – Triangle T(n,k)
 *
 * Formula:
 *   T(n,k) = T(n-1,k) + T(n-1,k-1)
 *            + ((n+1)*(n+2)*(2*n+3)/6) * T(n-2,k-1)
 *   with T(n,0) = T(n,n) = 1
 *
 * a(n) is the sequence obtained by reading the triangle row by row
 * (row 0: 1; row 1: 1,1; row 2: 1,16,1; …).
 *
 * The function memoises the rows of the triangle.  For any call to
 * a(n) (for n ≥ 0) it first ensures that the required rows are built,
 * then returns the appropriate entry.  The algorithm runs in O(n)
 * time for the first call and O(1) thereafter, and uses only standard
 * C11/C++17 library calls.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc, NULL */

/* External types supplied by the build environment */
extern seq_t unity;   /* dummy reference to keep seq_t visible */

/* Static storage for triangle rows */
static seq_t **rows = NULL;      /* array of pointers to rows */
static size_t rows_cap = 0;      /* capacity of the rows array */
static idx_t   rows_len = 0;     /* number of rows that have been computed */

/* Allocate or grow the rows array to hold indices up to 'r' (inclusive) */
static void need_rows_up_to(idx_t r)
{
    /* Grow rows[] if needed */
    if (rows_cap <= (size_t)r) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 1;
        while (new_cap <= (size_t)r) new_cap <<= 1;
        void *tmp = realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) return;                /* allocation failed – keep old array */
        rows = (seq_t **)tmp;
        rows_cap = new_cap;
    }

    /* Compute any missing rows */
    for (idx_t i = rows_len; i <= r; ++i) {
        /* Allocate storage for row i */
        void *tmp = malloc((i + 1) * sizeof(seq_t));
        if (!tmp) return;                /* allocation failed – abort building */
        seq_t *row = (seq_t *)tmp;

        /* Base cases: ends equal 1 */
        row[0] = 1;
        row[i] = 1;

        if (i >= 2) {
            /* Compute f(i) = (i+1)*(i+2)*(2*i+3)/6 */
            seq_t f = (seq_t)(i + 1);
            f *= (seq_t)(i + 2);
            f *= (seq_t)(2 * i + 3);
            f /= 6;

            for (idx_t k = 1; k < i; ++k) {
                seq_t term = rows[i - 1][k] + rows[i - 1][k - 1];
                term += f * rows[i - 2][k - 1];
                row[k] = term;
            }
        }

        rows[i] = row;
    }
    rows_len = r + 1;
}

/* Public function: returns a(n) for n >= 0 */
seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column k within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Build rows up to r if necessary */
    need_rows_up_to(r);

    /* If allocation failed, return 0 as a safe fallback */
    if (!rows || !rows[r]) return 0;

    return rows[r][k];
}

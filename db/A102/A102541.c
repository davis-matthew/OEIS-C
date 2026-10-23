/*
 * A102541  Triangle read by rows, formed from antidiagonals of Losanitsch's
 *          triangle. T(n, k) = A034851(n-k, k), n >= 0 and 0 <= k <=
 *          floor(n/2).
 * https://oeis.org/A102541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A102541: flattened Losanitsch's triangle
    T(n, k) is defined recursively:
            T(0,0) = 1,  T(n,0) = 1  (n ≥ 0)
            T(n,k) = 0 for k > ⌊n/2⌋
    For n ≥ 1, 0 ≤ k ≤ ⌊n/2⌋
            if n or k is even
                T(n,k) = T(n-1,k) + T(n-2,k-1)
            else   (n and k both odd)
                T(n,k) = T(n-1,k) + T(n-2,k-1)
                          – C((n-3)/2 – (k-1)/2 , (n-3)/2 – (k-1))
    The required sequence a(n) consists of the elements of T(n,k) read
    row‑wise.  The implementation builds the triangle lazily and
    caches all rows so that each value is computed only once.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t N, idx_t K); */

seq_t a(idx_t n)
{
    /* Cached data for incremental construction */
    static seq_t **rows = NULL;       /* rows[r] points to array of T(r,k) */
    static idx_t *row_start = NULL;  /* starting index of each row in the flattened array */
    static size_t rows_count = 0;    /* number of rows already built   */
    static size_t rows_cap   = 0;    /* current capacity of vectors     */
    static bool initialized = false;

    /* One‑time initialisation */
    if (!initialized) {
        rows_cap = 16;
        rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        row_start = (idx_t *)malloc(rows_cap * sizeof(idx_t));

        /* Row 0 */
        seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
        row0[0] = 1;
        rows[0] = row0;
        row_start[0] = 0;
        rows_count = 1;
        initialized = true;
    }

    /* Build rows until the one containing index n is available */
    while (true) {
        /* Determine the first index of the next row */
        idx_t prev_start  = row_start[rows_count - 1];
        idx_t prev_len    = (rows_count - 1) / 2 + 1;
        idx_t next_start  = prev_start + prev_len;

        if (next_start > n)
            break;   /* already have the required row */

        /* Grow storage if necessary */
        if (rows_count >= rows_cap) {
            rows_cap <<= 1;
            rows = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
            row_start = (idx_t *)realloc(row_start, rows_cap * sizeof(idx_t));
        }

        /* Compute new row */
        idx_t r = rows_count;
        size_t sz = r / 2 + 1;
        seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));

        for (size_t k = 0; k < sz; ++k) {
            if (k == 0) {
                curr[0] = 1;
                continue;
            }

            /* T(r-1,k) */
            seq_t val1 = 0;
            size_t sz1 = (r - 1) / 2 + 1;
            if (k < sz1)
                val1 = rows[r - 1][k];

            /* T(r-2,k-1) */
            seq_t val2 = 0;
            if (r >= 2 && k >= 1) {
                size_t sz2 = (r - 2) / 2 + 1;
                if (k - 1 < sz2)
                    val2 = rows[r - 2][k - 1];
            }

            seq_t val = val1 + val2;

            /* Subtract binomial term when r and k are both odd */
            if ((r & 1ULL) && (k & 1ULL)) {
                /* N = ((r-3)/2) - ((k-1)/2)  ,  B = ((r-3)/2) - (k-1) */
                idx_t N = ((r - 3) / 2) - ((k - 1) / 2);
                idx_t B = ((r - 3) / 2) - (k - 1);

                /* N and B are non‑negative and B ≤ N for valid positions */
                seq_t sub = binom(N, B);
                val -= sub;
            }

            curr[k] = val;
        }

        /* Store the new row and its starting index */
        rows[rows_count] = curr;
        row_start[rows_count] = next_start;
        ++rows_count;
    }

    /* Locate the row containing n */
    idx_t r = 0;
    while (r + 1 < rows_count && n >= row_start[r + 1])
        ++r;

    /* Column index within that row */
    size_t k = (size_t)(n - row_start[r]);

    return rows[r][k];
}

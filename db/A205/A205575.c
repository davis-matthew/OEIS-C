/*
 * A205575  Triangle read by rows, related to Pascal's triangle, starting with
 *          rows 1; 1,0.
 * https://oeis.org/A205575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n-1,k) + T(n-1,k-1) + T(n-2,k) + T(n-2,k-1) +
 *            T(n-2,k-2) with T(0,0)=1, T(1,0)=1, T(1,1)=0. The sequence a(n)
 *            is obtained by reading the triangle row‐wise
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage: seq_t a(idx_t n) */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined externally by the build. */
seq_t a(idx_t n)
{
    /* Static cache of rows.  rows[i] points to an array of i+1
       elements containing T(i,0)..T(i,i).  max_row tracks the
       highest row that has already been computed. */
    static seq_t **rows = NULL;
    static idx_t max_row = 0;

    /* Find the row r such that the n-th term lies in that row. */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Index of the term within row r. */
    idx_t prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - prev;

    /* Ensure that rows 0..r are computed. */
    if (rows == NULL) {
        rows = (seq_t **)malloc(((size_t)r + 1) * sizeof(seq_t *));
        for (idx_t i = 0; i <= r; ++i) {
            size_t len = (size_t)(i + 1);
            rows[i] = (seq_t *)malloc(len * sizeof(seq_t));
            if (i == 0) {                     /* row 0 : 1 */
                rows[0][0] = (seq_t)1;
            } else if (i == 1) {               /* row 1 : 1,0 */
                rows[1][0] = (seq_t)1;
                rows[1][1] = (seq_t)0;
            } else {                           /* compute using recurrence */
                seq_t *row_n  = rows[i];
                seq_t *row_n1 = rows[i - 1];
                seq_t *row_n2 = rows[i - 2];
                for (idx_t jk = 0; jk <= i; ++jk) {
                    seq_t val = 0;
                    if (jk <= i - 1)          /* T(i-1,jk)          */
                        val += row_n1[jk];
                    if (jk >= 1)               /* T(i-1,jk-1)        */
                        val += row_n1[jk - 1];
                    if (jk <= i - 2)          /* T(i-2,jk)          */
                        val += row_n2[jk];
                    if (jk >= 1 && jk <= i - 1) /* T(i-2,jk-1)      */
                        val += row_n2[jk - 1];
                    if (jk >= 2)               /* T(i-2,jk-2)        */
                        val += row_n2[jk - 2];
                    row_n[jk] = val;
                }
            }
        }
        max_row = r;
    } else if (r > max_row) {
        /* Extend cache to include rows up to r. */
        rows = (seq_t **)realloc(rows, ((size_t)r + 1) * sizeof(seq_t *));
        for (idx_t i = max_row + 1; i <= r; ++i) {
            size_t len = (size_t)(i + 1);
            rows[i] = (seq_t *)malloc(len * sizeof(seq_t));
            if (i == 0) {
                rows[0][0] = (seq_t)1;
            } else if (i == 1) {
                rows[1][0] = (seq_t)1;
                rows[1][1] = (seq_t)0;
            } else {
                seq_t *row_n  = rows[i];
                seq_t *row_n1 = rows[i - 1];
                seq_t *row_n2 = rows[i - 2];
                for (idx_t jk = 0; jk <= i; ++jk) {
                    seq_t val = 0;
                    if (jk <= i - 1)          /* T(i-1,jk)          */
                        val += row_n1[jk];
                    if (jk >= 1)               /* T(i-1,jk-1)        */
                        val += row_n1[jk - 1];
                    if (jk <= i - 2)          /* T(i-2,jk)          */
                        val += row_n2[jk];
                    if (jk >= 1 && jk <= i - 1) /* T(i-2,jk-1)      */
                        val += row_n2[jk - 1];
                    if (jk >= 2)               /* T(i-2,jk-2)        */
                        val += row_n2[jk - 2];
                    row_n[jk] = val;
                }
            }
        }
        max_row = r;
    }

    /* Return the requested term. */
    return rows[r][(size_t)k];
}

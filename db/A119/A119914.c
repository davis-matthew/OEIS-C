/*
 * A119914  Triangle read by rows: T(n,k) is number of ternary words of length
 *          n and having k runs of 0's of odd length (0 <= k <= ceiling(n/2);
 *          a run of 0's is a subsequence of consecutive 0's of maximal
 *          length).
 * https://oeis.org/A119914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = 2·T(n-1,k) + T(n-2,k) + 2·T(n-2,k-1) for n ≥ 2,
   with base T(0,0)=1, T(1,0)=2, T(1,1)=1.
   a(n) is the triangle read row‑by‑row; this function returns the
   value at linear index n. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for triangle rows and start indices */
    static seq_t **rows = NULL;          /* pointers to each row array */
    static idx_t *row_start = NULL;      /* start linear index of each row */
    static idx_t *row_len   = NULL;      /* number of elements in each row */
    static idx_t max_row = (idx_t)-1;    /* marker for uninitialized */
    static idx_t total_len = 0;          /* cumulative number of elements */

    /* ---- initialize the first row if not yet done ---- */
    if (max_row == (idx_t)-1) {
        max_row = 0;
        rows      = (seq_t**)malloc(sizeof(seq_t*));
        row_start = (idx_t*)malloc(sizeof(idx_t));
        row_len   = (idx_t*)malloc(sizeof(idx_t));

        /* row 0: length 1, value 1 */
        seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
        row0[0] = (seq_t)1;
        rows[0] = row0;
        row_start[0] = 0;
        row_len[0]   = 1;
        total_len = 1;   /* one element so far */
    }

    /* ---- generate rows until the requested index n is covered ---- */
    while (total_len <= n) {
        idx_t r = max_row + 1;                /* next row index */
        idx_t len = (r + 1) / 2 + 1;          /* number of k values */

        seq_t *row = (seq_t*)malloc(sizeof(seq_t) * len);

        if (r == 0) {                         /* row 0 already built */
            row[0] = (seq_t)1;
        }
        else if (r == 1) {                    /* base row 1 */
            row[0] = (seq_t)2;
            row[1] = (seq_t)1;
        }
        else {                                /* use recurrence for r ≥ 2 */
            idx_t cprev  = (r) / 2;           /* ceil((r-1)/2)   */
            idx_t cprev2 = (r - 1) / 2;       /* ceil((r-2)/2)   */
            for (size_t k = 0; k < (size_t)len; ++k) {
                seq_t val = 0;
                if (k <= (size_t)cprev) {          /* 2 * T(r-1,k) */
                    val += rows[r-1][k];
                    val += rows[r-1][k];
                }
                if (k <= (size_t)cprev2) {          /*   T(r-2,k)   */
                    val += rows[r-2][k];
                }
                if (k > 0 && (k-1) <= (size_t)cprev2) { /* 2 * T(r-2,k-1) */
                    val += rows[r-2][k-1];
                    val += rows[r-2][k-1];
                }
                row[k] = val;
            }
        }

        /* enlarge arrays to hold the new row */
        rows       = (seq_t**)realloc(rows,       sizeof(seq_t*) * (max_row + 2));
        row_start  = (idx_t*)realloc(row_start,  sizeof(idx_t)  * (max_row + 2));
        row_len    = (idx_t*)realloc(row_len,    sizeof(idx_t)  * (max_row + 2));

        rows[max_row + 1]      = row;
        row_start[max_row+1]   = total_len;
        row_len[max_row+1]     = len;

        total_len += len;
        max_row++;   /* we've just added row 'r' */
    }

    /* ---- locate the row containing index n ---- */
    idx_t r;
    for (r = 0; r <= max_row; ++r) {
        if (n < row_start[r] + row_len[r])
            break;
    }
    idx_t k = n - row_start[r];
    return rows[r][k];
}
